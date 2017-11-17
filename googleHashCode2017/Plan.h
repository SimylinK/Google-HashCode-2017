#pragma once

#include "Coordinate.h"
#include "Cell.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <vector>
#include <string>
#include <array>

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

	inline std::vector<Coordinate> getWires() { return wires; }
	inline std::vector<Coordinate> getRouters() { return routers; }

	void addRouter(Coordinate &c);
	void addBackbone(Coordinate &c);
	std::vector<Cell> reachableCells(const Coordinate &c);
	std::vector<Cell> coverableCells(const Coordinate &router);

	friend std::ostream &operator<<(std::ostream &os, const Plan &p);

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
