#include "Cell.h"

Cell::Cell(): wired(false), router(false), covered(false) {

}

Cell::Cell(Coordinate &coord, const char &c):wired(false), router(false), covered(false), coordinate(coord), environment(c) {

}

Cell::Cell(const Cell &c):wired(c.wired), router(c.router), covered(c.covered), coordinate(c.coordinate), environment(c.environment) {

}

Cell::Cell(Cell &&c):wired(c.wired), router(c.router), covered(c.covered), environment(c.environment) {
	coordinate = c.coordinate;
}

Cell &Cell::operator=(const Cell &c) {
	wired = c.wired;
	router = c.router;
	covered = c.covered;
	coordinate = c.coordinate;
	environment = c.environment;
	return *this;
}

Cell &Cell::operator=(Cell &&c) {
	wired = c.wired;
	router = c.router;
	covered = c.covered;
	coordinate = c.coordinate;
	environment = c.environment;
	return *this;
}

Cell::~Cell() {
}

int Cell::getNumberOfUncoveredCells() {
	auto uncoveredCells = getCoverableCells();
	return uncoveredCells.size();
}

std::vector<Cell *> Cell::getCoverableCells() {
	std::vector<Cell *> cells;
	for (auto& elem : coverableCells){
		if(!elem->isCovered()){
			cells.push_back(elem);
		}
	}
	return cells;
}