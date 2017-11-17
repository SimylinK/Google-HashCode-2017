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
	vector<Cell> covCells = coverableCells(c);
	for (Cell &cov:covCells) {
		Coordinate co = cov.getCoordinate();
		building[co.x][co.y].setCovered(true);
	}

}

void Plan::addWire(Coordinate &c) {
	wires.push_back(c);
	building[c.x][c.y].setWired(true);
}

/**
 *
 * @param router: coordinate where to place a router. If coordinate isn't a target cell, return is empty.
 * @return list of reachable target cells, including the one where the router is placed.
 */
vector<Cell> Plan::reachableCells(const Coordinate &router) {
	vector<Cell> cells;
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
						cells.push_back(building[i][j]);
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
vector<Cell> Plan::coverableCells(const Coordinate &router) {
	vector<Cell> reachableCells = this->reachableCells(router);
	vector<Cell> coverableCells;
	for (auto &e: reachableCells) {
		if (!e.isCovered()) {
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


/**
* Connect 2 routers with wire
* @param a: the router from wich we will begin
* @param b: the router wich is the destination
* @param money: the money used until now, will be upadta during this method
*/
void Plan::link(const Coordinate &a, const Coordinate &b, int &money) {
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
		vector<Coordinate> wiresToAdd;
		for (int i = 0; i < min(deltaX, deltaY); i++) {
			positionX += directionX;
			positionY += directionY;
			if (this->building[positionX][positionY].isWired()) {
				wiresToAdd.clear();
			}
			else {
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
				}
				else {
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
				}
				else {
					wiresToAdd.push_back(Coordinate(positionX, positionY));
				}
			}
		}
		else {
			throw std::exception("Plan::link la fonction n'a pas réussi à placer les cables");
		}

		for each (Coordinate wire in wiresToAdd)
		{
			this->addWire(wire);
			money += this->wireCost;
		}
	}
}


/**
* Find the barycentre of a list of Coordinate
* @param listCoord: the list of Coordinate from which to find the barycentre
* @return the barycentre of the list
*/
Coordinate& Plan::computeBarycentre(const std::vector<Coordinate> &listCoord) {
	int baryX = 0;
	int baryY = 0;

	for (Coordinate coord : listCoord) {
		baryX += coord.x;
		baryY += coord.y;
	}
	baryX /= listCoord.size();
	baryY /= listCoord.size();

	return Coordinate(baryX, baryY);
}



/**
* Find the coordinate object in a list that minimizes the distance with a coordinate object passed as parameter
* @param point: the point to minimize the distance with
* @param listCoord: a list of the points to minize the distance with
* @return the Coordinate with the minimal distance to the point
*/
Coordinate& Plan::argDistMin(const Coordinate &point, const std::vector<Coordinate> &listCoord) {
	if (listCoord.size() == 0) throw std::exception("Plan::argDistMin : La liste est vide");

	int min = distance(point, listCoord[0]);
	Coordinate argMin = listCoord[0];

	for (int index = 0; index < listCoord.size(); index++)
	{
		int dist = distance(point, listCoord[index]);
		if (dist < min) {
			min = dist;
			argMin = listCoord[index];
		}
	}
	return argMin;
}

/**
* Connect a list of routers to one of the barycentres
* @param listBarycentres: the barycentres where we connect the routers
* @param initialListRouters: list of the rooters to connect to the network
* @param money: the money used until now, will be upadta during this method
*/
void Plan::sectorLink(const std::vector<Coordinate> &listBarycentres, const std::vector<Coordinate> &initialListRouters, int &money) {
	Coordinate barycentre = computeBarycentre(initialListRouters);
	Coordinate closestBarycentre = argDistMin(barycentre, listBarycentres);
	link(closestBarycentre, barycentre, money);
	
	std::vector<Coordinate> routersToConnect = initialListRouters;

	while (routersToConnect.size() > 0) {
		std::vector<Coordinate> listConnectedRouters;
		recursiveLink(routersToConnect[0], initialListRouters, money, barycentre, listConnectedRouters);

		for each (Coordinate router in listConnectedRouters)
		{
			eraseFromVector(routersToConnect, router);
		}
	}
	
}


/**
* Connect a router to the network
* @param router: the router to be connected
* @param listRouters: list of the routers to work with, the method remove the rooter used from this list
* @param money: the money used until now, will be upadta during this method
* @param barycentre: the barycentre of listRouters
* @param listConnectedRouters: an empty list, this method will add all the routers connected during this method 
*/
void Plan::recursiveLink(const Coordinate &router, const std::vector<Coordinate> &listRouters, int &money, const Coordinate &barycentre, std::vector<Coordinate> &listConnectedRouters) {
	listConnectedRouters.push_back(router);
	std::vector<Coordinate> listCoordinatesForLink = listRouters;

	// Remove the router from  listCoordinatesForLink
	eraseFromVector(listCoordinatesForLink, router);

	listCoordinatesForLink.push_back(barycentre);

	// Find closest point to link
	Coordinate closestLinkablePoint = argDistMin(router, listCoordinatesForLink);

	if (!building[closestLinkablePoint.x][closestLinkablePoint.y].isWired()) {
		recursiveLink(closestLinkablePoint, listRouters, money, barycentre, listConnectedRouters);
	}

	closestLinkablePoint = followWire(closestLinkablePoint, router);

	link(closestLinkablePoint, router, money);
}

/**
* Find the closest connected point from a router to another
* @param startRouter: a router already connected
* @param targetRouter: the router to get closer from
* @return the coordinate of the connected point closest to the target
*/
Coordinate& Plan::followWire(const Coordinate &startRouter, const Coordinate &targetRouter) {
	Coordinate closestPoint = startRouter;

	// Try to get closer with the wires
	bool canGetCloser = true;
	Coordinate betterCoord1;
	Coordinate betterCoord2;
	Coordinate betterCoord3;

	do {
		// Get the 3 adjacent coordinates of closestPoint closer to the routeur
		if (closestPoint.x > targetRouter.x) {
			betterCoord1 = Coordinate(closestPoint.x - 1, closestPoint.y);
			if (closestPoint.y > targetRouter.y) {
				betterCoord2 = Coordinate(closestPoint.x - 1, closestPoint.y - 1);
				betterCoord3 = Coordinate(closestPoint.x, closestPoint.y - 1);
			}
			else if (closestPoint.y < targetRouter.y) {
				betterCoord2 = Coordinate(closestPoint.x - 1, closestPoint.y + 1);
				betterCoord3 = Coordinate(closestPoint.x, closestPoint.y + 1);
			}
			else if (closestPoint.y == targetRouter.y) {
				betterCoord2 = Coordinate(closestPoint.x - 1, closestPoint.y + 1);
				betterCoord3 = Coordinate(closestPoint.x - 1, closestPoint.y - 1);
			}
		}
		else if (closestPoint.x < targetRouter.x) {
			betterCoord1 = Coordinate(closestPoint.x + 1, closestPoint.y);
			if (closestPoint.y > targetRouter.y) {
				betterCoord2 = Coordinate(closestPoint.x + 1, closestPoint.y - 1);
				betterCoord3 = Coordinate(closestPoint.x, closestPoint.y - 1);
			}
			else if (closestPoint.y < targetRouter.y) {
				betterCoord2 = Coordinate(closestPoint.x + 1, closestPoint.y + 1);
				betterCoord3 = Coordinate(closestPoint.x, closestPoint.y + 1);
			}
			else if (closestPoint.y == targetRouter.y) {
				betterCoord2 = Coordinate(closestPoint.x + 1, closestPoint.y + 1);
				betterCoord3 = Coordinate(closestPoint.x + 1, closestPoint.y - 1);
			}
		}
		else if (closestPoint.x == targetRouter.x) {
			if (closestPoint.y > targetRouter.y) {
				betterCoord1 = Coordinate(closestPoint.x + 1, closestPoint.y - 1);
				betterCoord2 = Coordinate(closestPoint.x, closestPoint.y - 1);
				betterCoord3 = Coordinate(closestPoint.x - 1, closestPoint.y - 1);
			}
			else if (closestPoint.y < targetRouter.y) {
				betterCoord1 = Coordinate(closestPoint.x + 1, closestPoint.y + 1);
				betterCoord2 = Coordinate(closestPoint.x, closestPoint.y + 1);
				betterCoord3 = Coordinate(closestPoint.x - 1, closestPoint.y + 1);
			}
			else {
				canGetCloser = false;
			}
		}
		// Check if there is a wire at one of the 3 coordinates
		if (building[betterCoord1.x][betterCoord1.y].isWired()) closestPoint = betterCoord1;
		else if (building[betterCoord2.x][betterCoord2.y].isWired()) closestPoint = betterCoord2;
		else if (building[betterCoord3.x][betterCoord3.y].isWired()) closestPoint = betterCoord3;
		else canGetCloser = false;

	} while (canGetCloser);

	return closestPoint;
}


void Plan::eraseFromVector(std::vector<Coordinate> &vector, const Coordinate &coord) {
	std::vector<Coordinate>::iterator it;
	it = std::find(vector.begin(), vector.end(), coord);
	vector.erase(it);
}
