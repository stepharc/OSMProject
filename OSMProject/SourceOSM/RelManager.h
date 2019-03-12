#pragma once

#include "OsmWithOsmium.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/osm/relation.hpp>
#include <osmium/relations/relations_manager.hpp>

//Manager to get all the data a relation references to.
//The first template parameter of the RelationsManager is your class, the next three template parameters 
//tell the RelationsManager whether you are interested in member nodes, ways, and/or relations, respectively.
class RelManager : public osmium::relations::RelationsManager<RelManager, true, true, false> {
private:
	std::map<std::string, std::map<std::string, std::vector<std::string>>> dataToExtract_;
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> dataStorageRelations_;
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> dataStorageNodes_;
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> dataStorageWays_;

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

	//This function is called for every relation encountered in the OSM file.
	//If a relation meet the requirements (category/subcategory we are looking for), this relation
	//is "remembered" by the manager, for further processing.
	bool new_relation(const osmium::Relation& relation) noexcept {
		for (auto itcat = dataToExtract_.begin(); itcat != dataToExtract_.end(); itcat++) {
			// For each subcategory we want to search (example : residential).
			for (auto itsubcat = itcat->second.begin(); itsubcat != itcat->second.end(); itsubcat++) {
				// If the relation has a tag which match the current category and subcategory (ex : highway, residential)
				if (relation.tags().has_tag(itcat->first.c_str(), itsubcat->first.c_str())) {
					return true;
				}
			}
		}
		return false;
	}

	//This function is called for every member of every relation that has been "remembered"
	//by the function new_relation.
	//We express an interest for all of these members, so simply return true.
	bool new_member(const osmium::Relation& /*relation*/, const osmium::RelationMember& /*member*/, std::size_t /*n*/) noexcept {
		return true;
	}

	//This function is called for each relation we have expressed an interest in,
	//once all the members have been found in the input file.
	void complete_relation(const osmium::Relation& relation) {
		std::cout << "Relation id " << relation.id() << " is complete.\n";
		//Retrie type, subtype (if it exists) of this relation.
		OsmWithOsmium::insertValue(std::to_string(relation.id()), "type", relation.tags()["type"], dataStorageRelations_);
		const char* value = relation.tags()[relation.tags()["type"]];
		if(value) OsmWithOsmium::insertValue(std::to_string(relation.id()), relation.tags()["type"], value, dataStorageRelations_);
		//Iterate over all members
		for (const auto& member : relation.members()) {
			// member.ref() will be 0 for all members we are not interested
			// in. The objects for those members are not available.
			if (member.ref() != 0) {
				//Retrieve item type (way, node), reference and role of current member of this relation.
				std::string membertype(1, osmium::item_type_to_char(member.type()));
				OsmWithOsmium::insertValue(std::to_string(relation.id()), membertype, std::to_string(member.ref()), dataStorageRelations_);
				OsmWithOsmium::insertValue(std::to_string(relation.id()), std::to_string(member.ref()), member.role(), dataStorageRelations_);
				if (membertype == "w") { //member is a way
					const osmium::Way* way = this->get_member_way(member.ref());
					for (const osmium::NodeRef& nr : way->nodes()) {
						OsmWithOsmium::insertValue(std::to_string(way->id()), "noderef", std::to_string(nr.ref()), dataStorageWays_);
						OsmWithOsmium::insertValue(std::to_string(nr.ref()), "lon", std::to_string(nr.lon()), dataStorageNodes_);
						OsmWithOsmium::insertValue(std::to_string(nr.ref()), "lat", std::to_string(nr.lat()), dataStorageNodes_);
					}
				}
				else {
					if (membertype == "n") { //member is a node
						const osmium::Node* node = this->get_member_node(member.ref());
						OsmWithOsmium::insertValue(std::to_string(node->id()), "lon", std::to_string(node->location().lon()), dataStorageNodes_);
						OsmWithOsmium::insertValue(std::to_string(node->id()), "lat", std::to_string(node->location().lat()), dataStorageNodes_);
					}
				}
			}
		}
	}
};

