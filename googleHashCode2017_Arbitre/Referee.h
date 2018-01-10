#pragma once
#include <string>

#include "../googleHashCode2017/Plan.h"
#include "../googleHashCode2017/Cell.h"

class Referee {

	friend std::ostream &operator<<(std::ostream &o, Referee &Referee);

public:
	Referee() {};
	Referee(std::string repertoryName, std::string outputFile);
	Referee(const Referee&);
	~Referee() {};

	Referee &operator=(const Referee&);
	int calculateScore(); //calculates the score and returns it

private:
	bool testValidity();
	std::string splitOutput(std::string str);

	bool valid = true;
	int score = 0;
	int nbCellsConnected;
	int nbCellsCovered;
	int nbRouters;
	Coordinate backboneCell;
	Coordinate previousCell;
	std::vector<Coordinate> cellsCovered;
	std::vector<Coordinate> cellsConnected;
	std::vector<Coordinate> routers;
	Plan plan;

	std::string repertoryName;

};

std::ostream &operator<<(std::ostream &o, Referee &Referee);

bool areNeighbors(Coordinate c1, Coordinate c2);