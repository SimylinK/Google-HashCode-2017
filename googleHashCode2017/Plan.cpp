#include "Plan.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Plan::Plan()
{
}

Plan::Plan(string inputFile){

	string line;
	ifstream file(inputFile);
	if (file.is_open())
	{
		// first three lines have important info
		getline (file,line);
		vector<string> splitLine = splitString(line);
		rows = stoul(splitLine[0]);
		columns = stoul(splitLine[1]);
		routerRange = stoul(splitLine[2]);

		getline(file, line);
		splitLine = splitString(line);
		backboneCost = stoul(splitLine[0]);
		routerCost = stoul(splitLine[1]);
		maxBudget = stoul(splitLine[2]);

		getline(file, line);
		splitLine = splitString(line);
		backbones.push_back(Coordinate(stoul(splitLine[0]), stoul(splitLine[1])));

		building = new char*[rows];
		for (int i=0; i<rows; ++i){
			building[i] = new char[columns];
		}

		int rowNumber(0);
		while(getline(file, line)){
			for (unsigned j=0; j<line.length(); ++j)
			{
				building[rowNumber][j] = line.at(j);
			}
			rowNumber++;
		}

		file.close();
	} else {
		cout << "Unable to open input file : " << inputFile << endl;
	}

}


Plan::~Plan()
{
	for (int i=0; i<rows; ++i){
		delete [] building[i];
	}
	delete [] building;
}

vector<string> splitString(string str){
	string buf;
	stringstream ss(str);
	vector<string> tokens;
	while (ss >> buf)
		tokens.push_back(buf);

	return tokens;
}

std::ostream &operator<<(std::ostream &os, const Plan &p) {
	for (int i = 0; i < p.rows; ++i) {
		for (int j = 0; j < p.columns; ++j) {
			os << p.building[i][j];
		}
		os << endl;
	}
}