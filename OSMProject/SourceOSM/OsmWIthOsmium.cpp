#include "OsmWithOsmium.h"

#include <iostream>

void OsmWithOsmium::openFile(std::string filePath) {
	inputFile_ = osmium::io::File(filePath);
}

void OsmWithOsmium::readInputFile() {
	osmium::io::Reader reader(inputFile_);

	// OSM data comes in buffers, read until there are no more.
	while (osmium::memory::Buffer buffer = reader.read()) {
		std::cout << "Something has been read !\n";
	}
	std::cout << "Read done.\n";

	// You do not have to close the Reader explicitly, but because the
	// destructor can't throw, you will not see any errors otherwise.
	reader.close();
}

osmium::io::Header OsmWithOsmium::getInputFileHeader() {
	osmium::io::Reader reader(inputFile_);
	return reader.header();
}