#include "OsmWithOsmium.h"
#include "DataHandler.h"

#include <osmium/visitor.hpp>
#include <osmium/index/map/sparse_mem_array.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>

void OsmWithOsmium::openFile(std::string filePath) {
	inputFile_ = osmium::io::File(filePath);
}

void OsmWithOsmium::addDataToExtract(std::string category, std::string subcategory, std::vector<std::string> tags) {
	auto it = dataToExtract_.find(category);
	//This category has not been added yet.
	if (it == dataToExtract_.end()) {
		std::map<std::string, std::vector<std::string>> subcatTags;
		subcatTags.insert(std::pair<std::string, std::vector<std::string>>(subcategory, tags));
		dataToExtract_.insert(std::pair<std::string, std::map<std::string, std::vector<std::string>>>(category, subcatTags));
	}
	else {
		auto itsub = it->second.find(subcategory);
		//This subcategory has not been added yet.
		if (itsub == it->second.end()) {
			it->second.insert(std::pair<std::string, std::vector<std::string>>(subcategory, tags));
		}
		else {
			//Concatenate elements already in table with new tags passed in parameter.
			itsub->second.insert(itsub->second.end(), tags.begin(), tags.end());
		}
	}
}

void OsmWithOsmium::readInputFile() {
	//Optional unused argument to implement ? (Restrictive read)
	//osmium::osm_entity_bits::changeset 	Read changesets
	//osmium::osm_entity_bits::all 	Read all of the above
	osmium::io::Reader reader(inputFile_, osmium::osm_entity_bits::node | osmium::osm_entity_bits::way | osmium::osm_entity_bits::relation);

	//We need to call this function before apply, in order to have a restrictive search while reading.
	DataHandler handler;
	handler.setDataToExtract(dataToExtract_);

	//Handler in order to have node locations for ways references (all of them are nodes)
	namespace map = osmium::index::map;
	using index_type = map::SparseMemArray<osmium::unsigned_object_id_type, osmium::Location>;
	using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;

	index_type index;
	location_handler_type location_handler{ index };

	osmium::apply(reader, location_handler, handler);

	//Retrieve all the data we got from reading the input file.
	dataStorageNodes_ = handler.getDataStorageNodes();
	dataStorageWays_ = handler.getDataStorageWays();
	dataStorageRelations_ = handler.getDataStorageRelations();

	// We do not have to close the Reader explicitly, but because the
	// destructor can't throw, we will not see any errors otherwise.
	reader.close();
}

osmium::io::Header OsmWithOsmium::getInputFileHeader() {
	//Do not read any entities at all : useful when you are only interested in the file header.
	osmium::io::Reader reader(inputFile_, osmium::osm_entity_bits::nothing);
	return reader.header();
}

// Show data storage hashmaps content in console.
// For debug purposes only.
void OsmWithOsmium::printDataStorage(bool showNodes, bool showWays, bool showRelations) {
	if (showNodes) {
		std::cout << "----------- NODES -----------" << std::endl;
		for (auto it = dataStorageNodes_.begin(); it != dataStorageNodes_.end(); it++) {
			std::cout << "Node " << it->first << " :" << std::endl;
			for (auto pair : it->second) {
				std::cout << "\t" << pair.first << " : " << pair.second << std::endl;
			}
		}
	}
	if (showWays) {
		std::cout << "----------- WAYS -----------" << std::endl;
		for (auto it = dataStorageWays_.begin(); it != dataStorageWays_.end(); it++) {
			std::cout << "Way " << it->first << " :" << std::endl;
			for (auto pair : it->second) {
				std::cout << "\t" << pair.first << " : " << pair.second << std::endl;
			}
		}
	}
	if (showRelations) {
		std::cout << "----------- RELATIONS -----------" << std::endl;
		for (auto it = dataStorageRelations_.begin(); it != dataStorageRelations_.end(); it++) {
			std::cout << "Relation " << it->first << " :" << std::endl;
			for (auto pair : it->second) {
				std::cout << "\t" << pair.first << " : " << pair.second << std::endl;
			}
		}
	}
}