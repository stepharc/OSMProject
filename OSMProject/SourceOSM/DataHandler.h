#pragma once

#include <osmium/handler.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/osm/relation.hpp>
#include <iostream>
#include <algorithm>

class DataHandler : public osmium::handler::Handler {
private:
	enum Type { Way, Node, Relation };

	//Hashmap for restrictive osm file reading : in this example, we are looking for names and sources in ways, nodes and relations
	//in which there are a tag highway, residential.
	//Element example : ("highway", ("residential", {"name", "source"}))
	std::map<std::string, std::map<std::string, std::vector<std::string>>> dataToExtract_;

	//Hashmaps to store information, by node/way/relation id. For each id, we have a list of pairs :
	//left member : tag name, right member : its value.
	//Element example : ("1254125", {("name", "duschmoll"), ("source", "toto industries")})
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> dataStorageRelations_;
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> dataStorageNodes_;
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> dataStorageWays_;

	void insertValue(std::string id, std::string tagname, std::string tagvalue, std::map<std::string, std::vector<std::pair<std::string, std::string>>>& dataStorage) {
		auto it = dataStorage.find(id);
		// Way/Node/Relation with this id is not in the hashmap yet.
		if (it == dataStorage.end()) {
			std::vector<std::pair<std::string, std::string>> v{ std::pair<std::string, std::string>(tagname, tagvalue) };
			dataStorage.insert(std::pair < std::string, std::vector<std::pair<std::string, std::string>>>(id, v));
		}
		else {
			// Check if this pair is already in the hashmap.
			auto itbis = std::find_if(it->second.begin(), it->second.end(), [&tagname, &tagvalue](const std::pair<std::string, std::string> element) { return (element.first == tagname) && (element.second == tagvalue); });
			if (itbis == it->second.end()) {
				it->second.push_back(std::pair<std::string, std::string>(tagname, tagvalue));
			}
		}
	}

	bool storeOSMObject(const osmium::OSMObject& object, std::map<std::string, std::vector<std::pair<std::string, std::string>>>& dataStorage) {
		bool match = false;
		const osmium::TagList& tags = object.tags();
		// For each category we want to search (example : highway).
		for (auto itcat = dataToExtract_.begin(); itcat != dataToExtract_.end(); itcat++) {
		// For each subcategory we want to search (example : residential).
			for (auto itsubcat = itcat->second.begin(); itsubcat != itcat->second.end(); itsubcat++) {
				// If the node/way/relation has a tag which match the current category and subcategory (ex : highway, residential)
				if (tags.has_tag(itcat->first.c_str(), itsubcat->first.c_str())) {
					if (!match) match = true;
					for (std::string tagname : itsubcat->second) {
						// Store value if it is set.
						const char* value = tags[tagname.c_str()];
						if (value) {
							insertValue(std::to_string(object.id()), tagname, value, dataStorage);
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
			case Relation:
				match = storeOSMObject(object, dataStorageRelations_);
				break;
		}
		return match;
	}

public:
	void setDataToExtract(std::map<std::string, std::map<std::string, std::vector<std::string>>> dataToGet) {
		dataToExtract_ = dataToGet;
	}

	std::map<std::string, std::vector<std::pair<std::string, std::string>>> getDataStorageRelations() {
		return dataStorageRelations_;
	}

	std::map<std::string, std::vector<std::pair<std::string, std::string>>> getDataStorageNodes() {
		return dataStorageNodes_;
	}

	std::map<std::string, std::vector<std::pair<std::string, std::string>>> getDataStorageWays() {
		return dataStorageWays_;
	}

	void way(const osmium::Way& way) {
		bool match = storeData(way, Way);
		if (match) {
			//For each node reference, add its lontitude and latitude values to the nodes hashmap.
			for (const osmium::NodeRef& nr : way.nodes()) {
				insertValue(std::to_string(way.id()), "noderef", std::to_string(nr.ref()), dataStorageWays_);
				insertValue(std::to_string(nr.ref()), "lon", std::to_string(nr.lon()), dataStorageNodes_);
				insertValue(std::to_string(nr.ref()), "lat", std::to_string(nr.lat()), dataStorageNodes_);
			}
		}
	}

	void node(const osmium::Node& node) {
		bool match = storeData(node, Node);
		if (match) {
			// Retrieve lontitude and latitude for this node.
			insertValue(std::to_string(node.id()), "lon", std::to_string(node.location().lon()), dataStorageNodes_);
			insertValue(std::to_string(node.id()), "lat", std::to_string(node.location().lat()), dataStorageNodes_);
		}
	}

	void relation(const osmium::Relation& relation) {
		bool match = storeData(relation, Relation);
		if (match) {
			// Keep type of relation (boundary, ...) and for each member, its type (way, node, relation), id and role (admin_center, stop, "", ...)
			insertValue(std::to_string(relation.id()), "type", relation.tags()["type"], dataStorageRelations_);
			for (const osmium::RelationMember& rm : relation.members()) {
				std::string s(1, osmium::item_type_to_char(rm.type()));
				insertValue(std::to_string(relation.id()), s, std::to_string(rm.ref()), dataStorageRelations_);
				insertValue(std::to_string(relation.id()), std::to_string(rm.ref()), rm.role(), dataStorageRelations_);
			}
		}
	}
};
