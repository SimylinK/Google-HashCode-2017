#include "Referee.h"
#include "../googleHashCode2017/Utility.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <exception>

/**
* Determines if two cells are respectively neighbors, which means the distance between them equals 1 or less
* @param outputFile: a result file, corresponding to the specifications format
* @param inputFile: the input file used to create outputFile
*/

Referee::Referee(std::string outputFile, std::string inputFile) :
	plan(inputFile)
{
	std::string line;
	std::ifstream file(outputFile);

	if (file.is_open()) {

		try {
			getline(file, line);
			nbCellsConnected = stoi(line);

			std::vector<std::string> splitLine;
			Coordinate co;

			for (int i = 0; i < nbCellsConnected; i++) {

				getline(file, line);
				splitLine = splitString(line);

				co.x = stoi(splitLine[0]);
				co.y = stoi(splitLine[1]);
				cellsConnected.push_back(co);
			}

			getline(file, line);

			splitLine = splitString(line);
			if (splitLine.size() != 1) valid = false;

			nbRouters = stoi(line);

			for (int i = 0; i < nbRouters; i++) {

				getline(file, line);
				splitLine = splitString(line);

				co.x = stoi(splitLine[0]);
				co.y = stoi(splitLine[1]);
				routers.push_back(co);
			}

			file.close();


			//calculate how many cells are covered
			for (Coordinate &c : routers) {
				cellsCovered.push_back(c);
				std::vector<Cell*> covCells = plan.coverableCells(c);
				for (Cell *cov : covCells) {
					Coordinate co = cov->getCoordinate();
					cov->setCovered(true);
					cellsCovered.push_back(co);
				}
			}
			nbCellsCovered = cellsCovered.size();
		}
		catch (std::exception e) { //an exception is thrown if there is a char where there should be an int
			std::cout << "File format isn't correct" << std::endl;
			valid = false;
		}
	}
		
	else {
		std::cout << "Unable to open input file : " << outputFile << std::endl;
	}
}

Referee::Referee(const Referee& Referee):
	plan(Referee.plan)
{
	valid = Referee.valid;
	score = Referee.score;
	nbCellsConnected = Referee.nbCellsConnected;
	nbCellsCovered = Referee.nbCellsCovered;
	nbRouters = Referee.nbRouters;
	backboneCell = Referee.backboneCell;
	previousCell = Referee.previousCell;

	for (Coordinate co : cellsCovered) {
		cellsCovered.push_back(co);
	}

	for (Coordinate co : cellsConnected) {
		cellsConnected.push_back(co);
	}

	for (Coordinate co : routers) {
		routers.push_back(co);
	}
}

Referee &Referee::operator=(const Referee& Referee)
{
	valid = Referee.valid;
	score = Referee.score;
	nbCellsConnected = Referee.nbCellsConnected;
	nbCellsCovered = Referee.nbCellsCovered;
	nbRouters = Referee.nbRouters;
	backboneCell = Referee.backboneCell;
	previousCell = Referee.previousCell;

	for (Coordinate co : cellsCovered) {
		cellsCovered.push_back(co);
	}

	for (Coordinate co : cellsConnected) {
		cellsConnected.push_back(co);
	}

	for (Coordinate co : routers) {
		routers.push_back(co);
	}

	plan = Referee.plan;

	return *this;
}

/**
* Determines if two cells are respectively neighbors, which means the distance between them equals 1 or less
* @param c1: coordinates of the first cell (order doesn't matter)
* @param c2: coordinates of the second cell
* @return true if they are neighbors, else false
*/
bool areNeighbors(Coordinate c1, Coordinate c2) {
	bool areNeighors = false;

	if ((abs(c1.x - c2.x) <= 1)
		&& (abs(c1.y - c2.y) <= 1)) {

		areNeighors = true;
	}

	return areNeighors;
}

std::ostream &operator<<(std::ostream &o, Referee &Referee) {

	o << "Cells connected : " << std::endl;
	for (int i = 0; i < Referee.cellsConnected.size(); i++) {
		o << '(' << Referee.cellsConnected[i].x << ';' << Referee.cellsConnected[i].y << ')' << std::endl;
	}

	o << std::endl;

	o << "Routers : " << std::endl;
	for (int i = 0; i < Referee.routers.size(); i++) {
		o << '(' << Referee.routers[i].x << ';' << Referee.routers[i].y << ')' << std::endl;
	}

	return o;
}


/**
* Determines if the output file is valid
* @return true if it respects all the constraints of an output file, else false
*/
bool Referee::testValidity() {

	backboneCell = plan.getBackbone();
	previousCell = backboneCell;

	//each router has to be connected
	if (nbCellsConnected < nbRouters) {
		valid = false;
		std::cout << "nbCellsConnected < nbRouters" << std::endl;
	}

	//each cell has to be connected to the backbone or to the previous cell
	for (Coordinate &co : cellsConnected) {
		if (!areNeighbors(co, backboneCell) && !areNeighbors(co, previousCell)) {
			valid = false;
			std::cout << "a cell isn't connected to the backbone nor to the previous cell - " << co << "and the backbone is : " << backboneCell << std::endl;
		}

		previousCell = co;
	}

	//a router can't be on a wall cell nor on a void cell, only on a target cell .
	for (Coordinate &co : routers) {
		if ((plan(co.x, co.y)).floorType() != '.') {
			valid = false;
			std::cout << "a router is on a wall cell or on a void cell" << std::endl;
		}
	}

	//the budget max cannot be exceeded
	int budgetUsed = routers.size() * plan.getRouterCost() + cellsConnected.size() * plan.getWireCost();
	if (plan.getMaxBudget() < budgetUsed) {
		valid = false;
		std::cout << "the budget max is exceeded" << std::endl;
	}

	return valid;
}

/**
* Calculates the score corresponding to the output file and tests its validity
* @return the score
*/
int Referee::calculateScore() {

	int budget = plan.getMaxBudget();
	int wireCost = plan.getWireCost();
	int routerCost = plan.getRouterCost();

	testValidity();
	if (valid) score = 1000 * nbCellsCovered + (budget - nbCellsConnected * wireCost - nbRouters * routerCost);

	return score;
}