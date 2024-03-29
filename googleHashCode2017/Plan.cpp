#include "Plan.h"
#include "utility.h"
#include <iostream>
#include <fstream>

using namespace std;

Plan::Plan() {}

Plan::Plan(string inputFile) {
	std::cout << "--------------------Parsing--------------------" << std::endl;
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
			for (unsigned int j = 0; j < line.length(); ++j) {
				building[rowNumber][j].setCoordinate(Coordinate(rowNumber, j));
				building[rowNumber][j].setEnvironment(line.at(j));
			}
			rowNumber++;
		}

		file.close();

	} else {
		cout << "Unable to open input file : " << inputFile << endl;
		throw std::runtime_error("Bad name of file");
	}

	for(int i=0; i<rows; i++){
		for (int j=0; j<columns; j++){
			building[i][j].setCoverableCells(reachableCells(Coordinate(i, j))); // no need to check if the cells are covered, since there isn't any router yet
		}
	}
	spentMoney = 0;
}

Plan::Plan(const Plan &p) :
		rows(p.rows),
		columns(p.columns),
		routerCost(p.routerCost),
		routerRange(p.routerRange),
		maxBudget(maxBudget),
		routers(p.routers),
		wires(p.wires),
		backbone(p.backbone),
		spentMoney(p.spentMoney) {
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
					   backbone(p.backbone),
					   spentMoney(p.spentMoney) {
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
	spentMoney = p.spentMoney;
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
	spentMoney = p.spentMoney;

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
	// add the router to the grid
	building[c.x][c.y].setRouter(true);
	vector<Cell *> covCells = building[c.x][c.y].getCoverableCells();
	for (Cell *cov:covCells) {
		Coordinate co = cov->getCoordinate();
		cov->setCovered(true);
	}
	spentMoney += routerCost;
}

void Plan::addWire(Coordinate &c) {
	wires.push_back(c);
	building[c.x][c.y].setWired(true);
	spentMoney += wireCost;
}

/**
* Remove a number of routers and wires from the calling plan
* @param nbRouterSector: the number of routers to remove
* @param nbWires: the number of wires to remove
*/
void Plan::removeRouters(int nbRouterSector, int nbWires) {

	//Remove the routers
	Coordinate c;
	int i = 0;
	while (i < nbRouterSector && routers.size() > 0) {

		c = this->routers.back();
		if (this->building[c.x][c.y].hasRouter()) {
			this->building[c.x][c.y].setRouter(false);
			std::vector<Cell *> coverableCells = building[c.x][c.y].getCoverableCells();
			for (auto& elem : coverableCells) {
				elem->setCovered(false);
			}
		} else {
			throw std::invalid_argument("Plan::removeRouters : Tried to remove a router where there was not");
		}
		this->routers.pop_back();
		spentMoney -= routerCost;
		i++;
	}
	// Check that cells covered by an other router weren't set as not covered
	for (auto& coordinate : routers) {
		std::vector<Cell *> covCell = building[coordinate.x][coordinate.y].getCoverableCells();
		for (auto cell : covCell) {
			cell->setCovered(true);
		}
	}


	i = 0;
	//Remove the wires
	while (i < nbWires && wires.size() > 0) {
		c = this->wires.back();
		//std::cout << c;
		if (this->building[c.x][c.y].isWired()) {
			this->building[c.x][c.y].setWired(false);
		} else {
			throw std::invalid_argument("Plan::removeRouters :Tried to remove a wire where there was not");
		}
		this->wires.pop_back();
		spentMoney -= wireCost;
		i++;
	}
}


/**
 *
 * @param router: coordinate where to place a router. If coordinate isn't a target cell, return is empty.
 * @return list of reachable target cells, including the one where the router is placed.
 */
vector<Cell *> Plan::reachableCells(const Coordinate &router) {
	vector<Cell *> cells;
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
 * Function which prints the plan : In addition to Hashcode input conventions : R is a router, * is a covered cell, | is a wire
 * @param os: output stream
 * @param p: the plan to print
 * @return
 */
std::ostream &operator<<(std::ostream &os, const Plan &p) {
	for (int i = 0; i < p.rows; ++i) {
		for (int j = 0; j < p.columns; ++j) {
			if (i == p.backbone.x && j == p.backbone.y) {
				os << 'B';
			}
			if (p(i, j).hasRouter() && p(i,j).isWired()) {
				os << 'R';
			} else if(p(i, j).hasRouter() && !p(i,j).isWired()) {
				os << 'U';
			}
			else if (p(i, j).isWired()) {
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


/**
* Connect 2 routers with wire
* @param a: the router from which we will begin
* @param b: the router which is the destination
* @param money: the money used until now, will be updated during this method
*/
void Plan::link(const Coordinate &a, const Coordinate &b) {
	vector<Coordinate> wiresToAdd;

	if (a != b) {
		int deltaX = std::abs(b.x - a.x);
		if (deltaX == 0) deltaX = 1;
		int deltaY = std::abs(b.y - a.y);
		if (deltaY == 0) deltaY = 1;
		int directionX = (b.x - a.x) / deltaX;
		int directionY = (b.y - a.y) / deltaY;
		int positionX = a.x;
		int positionY = a.y;

		// Move diagonal
		for (int i = 0; i < min(deltaX, deltaY); i++) {
			positionX += directionX;
			positionY += directionY;
			if (this->building[positionX][positionY].isWired()) {
				wiresToAdd.clear();
			} else {
				wiresToAdd.push_back(Coordinate(positionX, positionY));
			}
		}
		//Move horizontal
		if (positionX == b.x) {
			int newDeltaY = std::abs(b.y - positionY);
			for (int i = 0; i < newDeltaY; i++) {
				positionY += directionY;
				if (this->building[positionX][positionY].isWired()) {
					wiresToAdd.clear();
				} else {
					wiresToAdd.push_back(Coordinate(positionX, positionY));
				}
			}
		}
			//Move vertical
		else if (positionY == b.y) {
			int newDeltaX = std::abs(b.x - positionX);
			for (int i = 0; i < newDeltaX; i++) {
				positionX += directionX;
				if (this->building[positionX][positionY].isWired()) {
					wiresToAdd.clear();
				} else {
					wiresToAdd.push_back(Coordinate(positionX, positionY));
				}
			}
		}

		for (Coordinate wire : wiresToAdd) {
			this->addWire(wire);
		}
	}
}

/**
 *
 * @return the percentage of covered target cells on the map
 */
double Plan::percentageCovered() {
	double numberTargetCells = 0;
	double numberCoveredCells = 0;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if(building[i][j].isCovered()){
				numberCoveredCells++;
			}
			if(building[i][j].floorType() == '.'){
				numberTargetCells++;
			}
		}
	}
	return numberCoveredCells/numberTargetCells*100;
}