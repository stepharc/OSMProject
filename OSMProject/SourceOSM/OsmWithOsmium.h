#pragma once

#include <string>
#include <osmium/io/any_input.hpp>

class OsmWithOsmium {
private:
	osmium::io::File inputFile_;

public:
	void openFile(std::string filePath);
	void readInputFile();
	osmium::io::Header getInputFileHeader();
};
