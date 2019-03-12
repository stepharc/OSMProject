#pragma once

#include <osmium/handler.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/osm/relation.hpp>
#include <iostream>
#include <algorithm>

class DataHandler : public osmium::handler::Handler {
private:
	enum Type { Way, Node };

	//Hashmap for restrictive osm file reading : in this example, we are looking for names and sources in ways and nodes
	//in which there are a tag highway, residential.
	//Element example : ("highway", ("residential", {"name", "source"}))
	std::map<std::string, std::map<std::string, std::vector<std::string>>> dataToExtract_;

	//Hashmaps to store information, by node/way id. For each id, we have a list of pairs :
	//left member : tag name, right member : its value.
	//Element example : ("1254125", {("name", "duschmoll"), ("source", "toto industries")})
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> dataStorageNodes_;
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> dataStorageWays_;

	bool storeOSMObject(const osmium::OSMObject& object, std::map<std::string, std::vector<std::pair<std::string, std::string>>>& dataStorage) {
		bool match = false;
		const osmium::TagList& tags = object.tags();
		// For each category we want to search (example : highway).
		for (auto itcat = dataToExtract_.begin(); itcat != dataToExtract_.end(); itcat++) {
		// For each subcategory we want to search (example : residential).
			for (auto itsubcat = itcat->second.begin(); itsubcat != itcat->second.end(); itsubcat++) {
				// If the node/way has a tag which match the current category and subcategory (ex : highway, residential)
				if (tags.has_tag(itcat->first.c_str(), itsubcat->first.c_str())) {
					if (!match) match = true;
					OsmWithOsmium::insertValue(std::to_string(object.id()), itcat->first.c_str(), itsubcat->first.c_str(), dataStorage);
					for (std::string tagname : itsubcat->second) {
						// Store value if it is set.
						const char* value = tags[tagname.c_str()];
						if (value) {
							OsmWithOsmium::insertValue(std::to_string(object.id()), tagname, value, dataStorage);
						}
					}
				}
			}
		}
		return match;
	}

	bool storeData(const osmium::OSMObject& object, Type type) {
		bool match;
		switch (type) {
			case Way:
				match = storeOSMObject(object, dataStorageWays_);
				break;
			case Node:
				match = storeOSMObject(object, dataStorageNodes_);
				break;
		}
		return match;
	}

public:
	void setDataToExtract(std::map<std::string, std::map<std::string, std::vector<std::string>>> dataToGet) {
		dataToExtract_ = dataToGet;
	}

	std::map<std::string, std::vector<std::pair<std::string, std::string>>> getDataStorageNodes() {
		return dataStorageNodes_;
	}

	std::map<std::string, std::vector<std::pair<std::string, std::string>>> getDataStorageWays() {
		return dataStorageWays_;
	}

	void setDataStorageNodes(std::map<std::string, std::vector<std::pair<std::string, std::string>>> dsn) {
		dataStorageNodes_ = dsn;
	}

	void setDataStorageWays(std::map<std::string, std::vector<std::pair<std::string, std::string>>> dsw) {
		dataStorageWays_ = dsw;
	}

	void way(const osmium::Way& way) {
		bool match = storeData(way, Way);
		if (match) {
			//For each node reference, add its lontitude and latitude values to the nodes hashmap.
			for (const osmium::NodeRef& nr : way.nodes()) {
				OsmWithOsmium::insertValue(std::to_string(way.id()), "noderef", std::to_string(nr.ref()), dataStorageWays_);
				OsmWithOsmium::insertValue(std::to_string(nr.ref()), "lon", std::to_string(nr.lon()), dataStorageNodes_);
				OsmWithOsmium::insertValue(std::to_string(nr.ref()), "lat", std::to_string(nr.lat()), dataStorageNodes_);
			}
		}
	}

	void node(const osmium::Node& node) {
		bool match = storeData(node, Node);
		if (match) {
			// Retrieve lontitude and latitude for this node.
			OsmWithOsmium::insertValue(std::to_string(node.id()), "lon", std::to_string(node.location().lon()), dataStorageNodes_);
			OsmWithOsmium::insertValue(std::to_string(node.id()), "lat", std::to_string(node.location().lat()), dataStorageNodes_);
		}
	}
};
