#include "OsmWithOsmium.h"
#include "DataHandler.h"

#include <osmium/visitor.hpp>

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
	//osmium::osm_entity_bits::relation 	Read relations
	//osmium::osm_entity_bits::changeset 	Read changesets
	//osmium::osm_entity_bits::all 	Read all of the above
	osmium::io::Reader reader(inputFile_, osmium::osm_entity_bits::node | osmium::osm_entity_bits::way);
	DataHandler handler;

	//We need to call this function before apply, in order to have a restrictive search while reading.
	handler.setDataToExtract(dataToExtract_);

	osmium::apply(reader, handler);

	//Retrieve all the data we got from reading the input file.
	dataStorage_ = handler.getDataStorage();

	// We do not have to close the Reader explicitly, but because the
	// destructor can't throw, we will not see any errors otherwise.
	reader.close();
}

osmium::io::Header OsmWithOsmium::getInputFileHeader() {
	//Do not read any entities at all : useful when you are only interested in the file header.
	osmium::io::Reader reader(inputFile_, osmium::osm_entity_bits::nothing);
	return reader.header();
}

// Shows data storage hashmap content in console.
// For debug purposes only.
void OsmWithOsmium::printDataStorage() {
	for (auto it = dataStorage_.begin(); it != dataStorage_.end(); it++) {
		std::cout << "Object " << it->first << " :" << std::endl;
		for (auto pair : it->second) {
			std::cout << "\t" << pair.first << " : " << pair.second << std::endl;
		}
	}
}