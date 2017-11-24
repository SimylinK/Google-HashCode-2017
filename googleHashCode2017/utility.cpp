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