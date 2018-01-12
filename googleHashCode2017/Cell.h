#ifndef GOOGLEHASHCODE2017_CELL_H
#define GOOGLEHASHCODE2017_CELL_H

#include <vector>
#include "Coordinate.h"
#include <iostream>

class Cell {

public:
	Cell();
	Cell(Coordinate &coord, const char &c);

	Cell(const Cell &);
	Cell(Cell &&);
	Cell &operator=(const Cell &);
	Cell &operator=(Cell &&);
	~Cell();

	// setters and getters
	inline bool isWired() { return wired; }
	inline bool hasRouter() { return router; }
	inline bool isCovered() { return covered; }
	inline void setWired(bool _wired) { wired = _wired; }
	inline void setRouter(bool _router) { router = _router; }
	inline void setCovered(bool _covered) { covered = _covered; }
	inline Coordinate getCoordinate() { return coordinate; }
	inline void setCoordinate(const Coordinate &c) { coordinate = c; }
	inline char floorType() { return environment; }
	inline void setEnvironment(const char &c) { environment = c; }
	inline void setCoverableCells(std::vector<Cell*> c) { coverableCells = c; }
	std::vector<Cell*> getCoverableCells();
	int getNumberOfUncoveredCells();

private:
	Coordinate coordinate;
	bool wired = false;
	bool router = false;
	bool covered = false; // true if the case floorType WiFi-covered
	char environment; //# or . or -
	std::vector<Cell*> coverableCells;  // the cells that would be covered if this cell was a router. They have to be rechecked because they can become covered, but there won't be any other cell than those
};

#endif //GOOGLEHASHCODE2017_CELL_H
