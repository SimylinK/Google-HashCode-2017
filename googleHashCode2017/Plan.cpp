#include "Plan.h"
#include "utility.h"
#include <iostream>
#include <fstream>

using namespace std;

Plan::Plan() {
}

Plan::Plan(string inputFile) {

	string line;
	ifstream file(inputFile);
	if (file.is_open()) {
		// first three lines have important info
		getline(file, line);
		vector<string> splitLine = splitString(line);
		rows = stoi(splitLine[0]);
		columns = stoi(splitLine[1]);
		routerRange = stoi(splitLine[2]);

		getline(file, line);
		splitLine = splitString(line);
		wireCost = stoi(splitLine[0]);
		routerCost = stoi(splitLine[1]);
		maxBudget = stoi(splitLine[2]);

		getline(file, line);
		splitLine = splitString(line);
		backbone = Coordinate(stoi(splitLine[0]), stoi(splitLine[1]));

		building = new Cell *[rows];
		for (int i = 0; i < rows; ++i) {
			building[i] = new Cell[columns];
		}

		int rowNumber(0);
		while (getline(file, line)) {
			for (int j = 0; j < line.length(); ++j) {
				building[rowNumber][j].setCoordinate(Coordinate(rowNumber, j));
				building[rowNumber][j].setEnvironment(line.at(j));
			}
			rowNumber++;
		}

		file.close();
	} else {
		cout << "Unable to open input file : " << inputFile << endl;
	}

}

Plan::Plan(const Plan &p) :
		rows(p.rows),
		columns(p.columns),
		routerCost(p.routerCost),
		routerRange(p.routerRange),
		maxBudget(maxBudget),
		routers(p.routers),
		wires(p.wires),
		backbone(p.backbone) {
	building = new Cell *[rows];
	for (int i = 0; i < rows; ++i) {
		building[i] = new Cell[columns];
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			building[i][j] = p(i, j);
		}
	}

}

Plan::Plan(Plan &&p) : rows(p.rows),
					   columns(p.columns),
					   routerCost(p.routerCost),
					   routerRange(p.routerRange),
					   maxBudget(maxBudget),
					   routers(p.routers),
					   wires(p.wires),
					   backbone(p.backbone) {
	building = p.building;
	p.building = nullptr;

}

Plan &Plan::operator=(const Plan &p) {
	columns = p.columns;
	routerCost = p.routerCost;
	routerRange = p.routerRange;
	maxBudget = p.maxBudget;
	routers = p.routers;
	wires = p.wires;
	backbone = p.backbone;
	// delete building
	for (int i = 0; i < rows; ++i) {
		delete[] building[i];
	}
	delete[] building;

	// recreate it
	building = new Cell *[rows];
	for (int i = 0; i < rows; ++i) {
		building[i] = new Cell[columns];
	}

	// copy values
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			building[i][j] = p(i, j);
		}
	}

	return *this;
}

Plan &Plan::operator=(Plan &&p) {
	columns = p.columns;
	routerCost = p.routerCost;
	routerRange = p.routerRange;
	maxBudget = p.maxBudget;
	routers = p.routers;
	wires = p.wires;
	backbone = p.backbone;

	// delete building
	for (int i = 0; i < rows; ++i) {
		delete[] building[i];
	}
	delete[] building;

	building = p.building;
	p.building = nullptr;

	return *this;
}

Plan::~Plan() {
	for (int i = 0; i < rows; ++i) {
		delete[] building[i];
	}
	delete[] building;
}

void Plan::addRouter(Coordinate &c) {
	routers.push_back(c);
	building[c.x][c.y].setRouter(true);
	vector<Cell*> covCells = coverableCells(c);
	for (Cell *cov:covCells) {
		Coordinate co = cov->getCoordinate();
		cov->setCovered(true);
	}

}

void Plan::addWire(Coordinate &c) {
	wires.push_back(c);
}

/**
 *
 * @param router: coordinate where to place a router. If coordinate isn't a target cell, return is empty.
 * @return list of reachable target cells, including the one where the router is placed.
 */
vector<Cell*> Plan::reachableCells(const Coordinate &router) {
	vector<Cell*> cells;
	if (building[router.x][router.y].floorType() == '.') {

		// cases to check
		int minX = (router.x - routerRange < 0) ? 0 : router.x - routerRange;
		int maxX = (router.x + routerRange >= rows) ? rows - 1 : router.x + routerRange;
		int minY = (router.y - routerRange < 0) ? 0 : router.y - routerRange;
		int maxY = (router.y + routerRange >= columns) ? columns - 1 : router.y + routerRange;

		for (int i = minX; i <= maxX; i++) {
			for (int j = minY; j <= maxY; j++) {
				if (building[i][j].floorType() == '.') {
					// smaller rectangle between router and target
					int minXW = min(i, router.x);
					int maxXW = max(i, router.x);
					int minYW = min(j, router.y);
					int maxYW = max(j, router.y);
					bool wall = false;
					for (int wx = minXW; wx <= maxXW && !wall; wx++) {
						for (int wy = minYW; wy <= maxYW && !wall; wy++) {
							if (building[wx][wy].floorType() == '#') {
								wall = true;
							}
						}
					}
					if (!wall) {
						cells.push_back(&building[i][j]);
					}
				}

			}
		}
	}
	return cells;
}

/**
 *
 * @param router: coordinate where to place a router. If coordinate isn't a target cell, return is empty.
 * @return list of coverable target cells, including the one where the router is placed. A coverable cell is a reachable cell that is not covered yet.
 */
vector<Cell*> Plan::coverableCells(const Coordinate &router) {
	vector<Cell*> reachableCells = this->reachableCells(router);
	vector<Cell*> coverableCells;
	for (auto &e: reachableCells) {
		if (!e->isCovered()) {
			coverableCells.push_back(e);
		}
	}
	return coverableCells;
}

/**
 * Function which prints the plan : In addition to Hashcode input conventions : R is a router, * is a covered cell, | is a wire
 * @param os: output stream
 * @param p: the plan to print
 * @return
 */
std::ostream &operator<<(std::ostream &os, const Plan &p) {
	for (int i = 0; i < p.rows; ++i) {
		for (int j = 0; j < p.columns; ++j) {
			if (p(i, j).hasRouter()) {
				os << 'R';
			} else if (p(i, j).isWired()) {
				os << '|';
			} else if (p(i, j).isCovered()) {
				os << '*';
			} else {
				os << p(i, j).floorType();
			}
		}
		os << endl;
	}
	return os;
}
