#pragma once

#include <osmium/handler.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
//#include <osmium/osm/relation.hpp>
#include <iostream>

class DataHandler : public osmium::handler::Handler {
private:
	//Hashmap for restrictive osm file reading : in this example, we are looking for names and sources in ways and nodes
	//in which there are a tag highway, residential.
	//Element example : ("highway", ("residential", {"name", "source"}))
	std::map<std::string, std::map<std::string, std::vector<std::string>>> dataToExtract_;

	//Hashmap to store information, by node/way id. For each id, we have a list of pairs :
	//left member : tag name, right member : its value.
	//Element example : ("1254125", {("name", "duschmoll"), ("source", "toto industries")})
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> dataStorage_;

	void insertValue(std::string id, std::string tagname, std::string tagvalue) {
		auto it = dataStorage_.find(id);
		// Way/Node with this id is not in the hashmap yet.
		if (it == dataStorage_.end()) {
			std::vector<std::pair<std::string, std::string>> v{ std::pair<std::string, std::string>(tagname, tagvalue) };
			dataStorage_.insert(std::pair < std::string, std::vector<std::pair<std::string, std::string>>>(id, v));
		}
		else {
			it->second.push_back(std::pair<std::string, std::string>(tagname, tagvalue));
		}
	}

	void storeData(const osmium::OSMObject& object) {
		const osmium::TagList& tags = object.tags();
		// For each category we want to search (example : highway).
		for (auto itcat = dataToExtract_.begin(); itcat != dataToExtract_.end(); itcat++) {
			// For each subcategory we want to search (example : residential).
			for (auto itsubcat = itcat->second.begin(); itsubcat != itcat->second.end(); itsubcat++) {
				// If the node or the way has a tag which match the current category and subcategory (ex : highway, residential)
				if (tags.has_tag(itcat->first.c_str(), itsubcat->first.c_str())) {
					for (std::string tagname : itsubcat->second) {
						// Store value if it is set.
						const char* value = tags[tagname.c_str()];
						if (value) {
							insertValue(std::to_string(object.id()), tagname, value);
						}
					}
				}
			}
		}
	}

public:
	void setDataToExtract(std::map<std::string, std::map<std::string, std::vector<std::string>>> dataToGet) {
		dataToExtract_ = dataToGet;
	}

	std::map<std::string, std::vector<std::pair<std::string, std::string>>> getDataStorage() {
		return dataStorage_;
	}

	void way(const osmium::Way& way) {
		/*std::cout << "way " << way.id() << std::endl;
		for (const osmium::NodeRef& nr : way.nodes()) {
			std::cout << "waynoderef " << nr.ref() << std::endl;
		}
		for (const osmium::Tag& t : way.tags()) {
			std::cout << "waytag " << t.key() << "=" << t.value() << std::endl;
		}*/
		storeData(way);
	}

	void node(const osmium::Node& node) {
		/*std::cout << "node " << node.id() << " : " << node.location() << std::endl;
		// Not used yet ? : node.location().x(), node.location().y(), node.location().lon(), node.location().lat().
		for (const osmium::Tag& t : node.tags()) {
			std::cout << "nodetag " << t.key() << "=" << t.value() << std::endl;
		}*/
		storeData(node);
	}

	/*void relation(const osmium::Relation& relation) {
		std::cout << "relation " << relation.id() << std::endl;
		for (const osmium::RelationMember& rm : relation.members()) {
			std::cout << "relationmember " << rm.type() << ": " << rm.ref() << "=" << rm.role() << std::endl;
		}
		for (const osmium::Tag& t : relation.tags()) {
			std::cout << "relationtag " << t.key() << "=" << t.value() << std::endl;
		}
	}*/
};
