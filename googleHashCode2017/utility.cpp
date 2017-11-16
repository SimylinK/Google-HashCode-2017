#include <fstream>
#include "utility.h"

std::vector<std::string> splitString(std::string str){
	std::string buf;
	std::stringstream ss(str);
	std::vector<std::string> tokens;
	while (ss >> buf)
		tokens.push_back(buf);

	return tokens;
}

std::pair<int, int> readDimensions(const char* inputFile){
	std::pair<int, int> dimensions;
	std::string line;
	std::ifstream file(inputFile);
	if (file.is_open()) {
		// first three lines have important info
		getline(file, line);
		std::vector<std::string> splitLine = splitString(line);
		dimensions.first = stoi(splitLine[0]);
		dimensions.second = stoi(splitLine[1]);
		return dimensions;
	} else {
		throw std::exception();
	}
}