#pragma once

#include <string>
#include <osmium/io/any_input.hpp>

class OsmWithOsmium {
private:
	osmium::io::File inputFile_;
	std::map<std::string, std::map<std::string, std::vector<std::string>>> dataToExtract_;
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> dataStorage_;

public:
	void openFile(std::string filePath);
	void readInputFile();
	void addDataToExtract(std::string category, std::string subcategory, std::vector<std::string> tags);
	osmium::io::Header getInputFileHeader();
	void printDataStorage();
};