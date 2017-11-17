#pragma once

#include "Coordinate.h"
#include "Cell.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <vector>
#include <string>
#include <array>
#include <list>
#include <algorithm>

class Plan {
public:
	Plan();
	Plan(std::string);
	Plan(const Plan&);
	Plan(Plan&&);
	Plan& operator=(const Plan&);
	Plan& operator=(Plan&&);
	~Plan();

	inline Cell& operator()(int i, int j){return building[i][j];}
	inline Cell& operator()(int i, int j) const {return building[i][j];}

	inline int getRows() { return rows; }
	inline int getColumns() { return columns; }
	inline int getRouterRange(){return routerRange;}
	inline int getWireCost(){return wireCost;}
	inline int getRouterCost(){return routerCost;}
	inline int getMaxBudget(){return maxBudget;}

	void addRouter(Coordinate &c);
	void addWire(Coordinate &c);
	std::vector<Cell> reachableCells(const Coordinate &c);
	std::vector<Cell> coverableCells(const Coordinate &router);

	friend std::ostream &operator<<(std::ostream &os, const Plan &p);

	// For the first solver
	void link(const Coordinate &a, const Coordinate &b, const bool &stopAtFirstWire, int &money);
	Coordinate& computeBarycentre(const std::vector<Coordinate> &listCoord);
	Coordinate& argDistMin(const Coordinate &point, const std::vector<Coordinate> &listCoord);

	void sectorLink(const std::vector<Coordinate> &listBarycentres, const std::vector<Coordinate> &initialListRouters, int &money);
	void recursiveLink(const Coordinate &router, const std::vector<Coordinate> &listRouters, int &money, const Coordinate &barycentre, std::vector<Coordinate> &listConnectedRouters);
	Coordinate& followWire(const Coordinate &startRouter, const Coordinate &targetRouter);
	void eraseFromVector(std::vector<Coordinate> &vector, const Coordinate &coord);

private :
	int rows;
	int columns;
	int routerRange;
	int wireCost; // cost to link one cell to backbone
	int routerCost; // cost to place one wireless router
	int maxBudget;
	Cell **building;
	std::vector<Coordinate> routers;
	Coordinate backbone;
	std::vector<Coordinate> wires;
};

std::ostream &operator<<(std::ostream &os, const Plan &p);
