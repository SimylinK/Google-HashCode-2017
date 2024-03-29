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
	friend std::ostream &operator<<(std::ostream &os, const Plan &p);
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
	inline int getSpentMoney(){return spentMoney;}
	void setSpentMoney(int m){spentMoney = m;}
	inline Coordinate getBackbone() { return backbone; };

	inline bool isWired(const Coordinate &coord) { return building[coord.x][coord.y].isWired(); };

	inline std::vector<Coordinate> getWires() { return wires; }
	void setWires(std::vector<Coordinate>& w) { wires = w; }

	inline std::vector<Coordinate> getRouters() { return routers; }
	void setRouters(std::vector<Coordinate>& r) { routers = r; }



	void addRouter(Coordinate &c);
	void addWire(Coordinate &c);
	void removeRouters(int nbRouterSector, int nbWires);
	std::vector<Cell*> reachableCells(const Coordinate &c);
	void link(const Coordinate &a, const Coordinate &b);
	double percentageCovered();

private :
	int rows;
	int columns;
	int routerRange;
	int wireCost; // cost to link one cell to backbone
	int routerCost; // cost to place one wireless router
	int maxBudget;
	int spentMoney;
	Cell **building;
	std::vector<Coordinate> routers;
	Coordinate backbone;
	std::vector<Coordinate> wires;
};

std::ostream &operator<<(std::ostream &os, const Plan &p);
