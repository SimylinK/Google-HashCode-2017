#include <cmath>
#include "Solver.h"
#include <map>
#include <iostream>

/**
* Position and link a network of routers on a plan to maximise the number of covered cells
* @param p: the plan to position the routers on
* @param nbRouterSector: the maximum number of routers in a sector
* @return the money spent
*/
void solveProblem(Plan &p) {

	std::cout << "--------------------Positioning routers--------------------" << std::endl;
	placeRoutersIterative(p);
	std::cout << "--------------------Positioning wires--------------------" << std::endl;

	std::vector<Coordinate> routers = p.getRouters();
	gridWiring(routers, p);
	std::cout << "--------------------fillingBlanks--------------------" << std::endl;
	fillBlanks(p);
}


/**
* Wire a list of router sectors and return the list of neighboring router sectors
* @param listSectorRouters: the list of pair of indices (i,j) corresponding to the sectors to wire
* @param p: the plan where the routers are
* @param spentMoney : the money used until now, will be updated during this method
* @return the list of neighboring router sector, not wired yet
*/
void gridWiring(std::vector<Coordinate> &routers, Plan &p) {
	Coordinate currentBary;

	std::vector<Coordinate> listConnectedRouters;
	std::list<std::vector<Coordinate>> listWiredNeighbors;
	std::pair<Coordinate, Coordinate> closestRoutersPair;

	for (auto& router : routers) {

		listConnectedRouters.clear();
		listWiredNeighbors = { std::vector<Coordinate>({ p.getBackbone() }) };

		currentBary = computeBarycentre(routers);
		routers.push_back(currentBary);


		closestRoutersPair = linkTwoGroups(p,listWiredNeighbors, routers);

		p.link(closestRoutersPair.first, closestRoutersPair.second);
		recursiveLink(p, closestRoutersPair.second, routers, currentBary, listConnectedRouters, true);

		sectorLink(p, routers, listConnectedRouters, false);

	}
}


/**
* Position all possible routers with maximum coverage area
* @param p: the plan to position the routers on
* @param spentMoney : the money used until now, will be updated during this method
*/
void placeRoutersIterative(Plan &p) {
	int maxReachableCells = (p.getRouterRange() * 2 + 1) * (p.getRouterRange() * 2 + 1);
	int routerCost = p.getRouterCost();

	for (int i = 0; i < p.getRows() && p.getSpentMoney() < p.getMaxBudget() - routerCost; i++) {
		for (int j = 0; j < p.getColumns() && p.getSpentMoney() < p.getMaxBudget() - routerCost; j++) {
			if (p(i, j).getCoverableCells()>= maxReachableCells) {
				Coordinate c(i, j);
				p.addRouter(c);
			}
		}
	}
}

void fillBlanks(Plan &p){
	int maxReachableCells = (p.getRouterRange() * 2 + 1) * (p.getRouterRange() * 2 + 1);
	int routerCost = p.getRouterCost();

	for (int reachableCells = maxReachableCells -1; reachableCells>0; reachableCells--) {
		int nbRouters = 0;
		for (int i = 0; i < p.getRows() && p.getSpentMoney() < p.getMaxBudget() - routerCost; i++) {
			for (int j = 0; j < p.getColumns() && p.getSpentMoney() < p.getMaxBudget() - routerCost; j++) {
				if (p(i, j).getCoverableCells() == reachableCells) {
					Coordinate c(i, j);
					std::vector<Coordinate> unwiredGroup;
					std::vector<Coordinate> wiredGroup = p.getRouters(); // add routers already present in that part of the grid
					// then add cables present in this part of the grid
					for (auto& wire : p.getWires()){
						wiredGroup.push_back(wire);
					}
					unwiredGroup.push_back(c);
					std::list<std::vector<Coordinate>> listWiredGroup;
					listWiredGroup.push_back(wiredGroup);
					linkTwoGroups(p, listWiredGroup, unwiredGroup);
					p.addRouter(c);
					nbRouters++;
				}
			}
		}
		std::cout << "Positionned " << nbRouters << " routers that cover " << reachableCells << " cells" << std::endl;
	}
}

/**
* Find the barycentre of a list of Coordinate
* @param listCoord: the list of Coordinate from which to find the barycentre
* @return the barycentre of the list
*/
Coordinate computeBarycentre(const std::vector<Coordinate> &listCoord) {
	int baryX = 0;
	int baryY = 0;

	for (Coordinate coord : listCoord) {
		baryX += coord.x;
		baryY += coord.y;
	}
	baryX /= static_cast<int>(listCoord.size());
	baryY /= static_cast<int>(listCoord.size());

	return Coordinate(baryX, baryY);
}

/**
* Find the coordinate object in a list that minimizes the distance with a coordinate object passed as parameter
* @param point: the point to minimize the distance with
* @param listCoord: a list of the points to minize the distance with
* @return the Coordinate with the minimal distance to the point
*/
Coordinate argDistMin(const Coordinate &point, const std::vector<Coordinate> &listCoord) {
	if (listCoord.size() == 0) throw std::invalid_argument("Plan::argDistMin : La liste est vide");

	int min = distance(point, listCoord[0]);
	Coordinate argMin = listCoord[0];

	for (unsigned int index = 0; index < listCoord.size(); index++) {
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
* @param plan: the plan to work with
* @param initialListRouters: list of the rooters to connect to the network
* @param money: the money used until now, will be upadta during this method
*/

void sectorLink(Plan &plan, std::vector<Coordinate> &initialListRouters,
				std::vector<Coordinate> &listConnectedRouters,
				bool reversedMode) {

	Coordinate barycentre = computeBarycentre(initialListRouters);
	std::vector<Coordinate> routersToConnect = initialListRouters;

	while (routersToConnect.size() > 0) {

		recursiveLink(plan, routersToConnect[0], initialListRouters, barycentre, listConnectedRouters, reversedMode);

		for (Coordinate router : listConnectedRouters) {
			eraseFromVector(routersToConnect, router);
		}
		listConnectedRouters.clear();
	}

}


/**
* Do the shortest connection between a router sector and a list of router sector
* @param listWiredGroup: a list of router sector already connected
* @param unwiredGroup: a router sector of coordinate not connected
* @param p: the plan to work with
* @return the pair of coordinates that minimizes the distance between unwiredGroup and listWiredGroup
*/
std::pair<Coordinate, Coordinate> linkTwoGroups(Plan &p, const std::list<std::vector<Coordinate>>& listWiredGroup, const std::vector<Coordinate>& unwiredGroup)
{
	Coordinate bestWiredToConnect;
	bool foundNonEmptyGroup = false;
	for (auto it = listWiredGroup.begin(); it != listWiredGroup.end() && !foundNonEmptyGroup;it++){
		if ((*it).size() != 0) {
			bestWiredToConnect = (*it)[0];
			foundNonEmptyGroup = true;
		}
	}
	Coordinate bestUnwiredToConnect = unwiredGroup[0];
	
	int bestDistance = distance(bestWiredToConnect, bestUnwiredToConnect);
	for (auto &wiredGroup : listWiredGroup) {
		for (auto &wiredCoord : wiredGroup) {
			for (auto &unwiredCoord : unwiredGroup) {
				if (distance(wiredCoord, unwiredCoord) < bestDistance) {
					bestWiredToConnect = wiredCoord;
					bestUnwiredToConnect = unwiredCoord;
					bestDistance = distance(bestWiredToConnect, bestUnwiredToConnect);
				}
			}
		}
	}
	p.link(bestWiredToConnect, bestUnwiredToConnect);
	return std::pair<Coordinate, Coordinate>(bestWiredToConnect,bestUnwiredToConnect);
}

/**
* Connect a router to the network
* @param router: the router to be connected
* @param listRouters: list of the routers to work with, the method removes the wired routers from this list
* @param barycentre: the barycentre of listRouters
* @param listConnectedRouters: an empty list, this method will add all the routers connected during this method
*/
void recursiveLink(Plan &plan, const Coordinate &router, const std::vector<Coordinate> &listRouters,
				   const Coordinate &barycentre, std::vector<Coordinate> &listConnectedRouters,
				   bool reversedMode = false) {


	listConnectedRouters.push_back(router);
	std::vector<Coordinate> listCoordinatesForLink = listRouters;

	eraseFromVector(listCoordinatesForLink, router);
	//If the barycentre is already in listCoordinatesForLink, no need to add it again
	if (std::find(listCoordinatesForLink.begin(), listCoordinatesForLink.end(), barycentre) ==
		listCoordinatesForLink.end()) {
		listCoordinatesForLink.push_back(barycentre);
	}

	//router is going to be linked with the closest point (a router or the barycentre)
	Coordinate closestLinkablePoint = argDistMin(router, listCoordinatesForLink);

	if (reversedMode) {
		closestLinkablePoint = followWire(plan, closestLinkablePoint, router);
		plan.link(router, closestLinkablePoint);
	}

	if ((!plan.isWired(closestLinkablePoint) || reversedMode) && router != barycentre) {
		recursiveLink(plan, closestLinkablePoint, listCoordinatesForLink, barycentre, listConnectedRouters,
					  reversedMode);
	}


	if (!reversedMode) {

		closestLinkablePoint = followWire(plan, closestLinkablePoint, router);
		plan.link(closestLinkablePoint, router);
	}

}

/**
* Find the closest connected point from a router to another
* @param startRouter: a router already connected
* @param targetRouter: the router to get closer from
* @return the coordinate of the connected point closest to the target
*/
Coordinate followWire(Plan &plan, const Coordinate &startRouter, const Coordinate &targetRouter) {

	Coordinate closestPoint = startRouter;

	if (plan.isWired(startRouter)) { //If the point is not wired, it cannot get closer
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
				} else if (closestPoint.y < targetRouter.y) {
					betterCoord2 = Coordinate(closestPoint.x - 1, closestPoint.y + 1);
					betterCoord3 = Coordinate(closestPoint.x, closestPoint.y + 1);
				} else if (closestPoint.y == targetRouter.y) {
					betterCoord2 = Coordinate(closestPoint.x - 1, closestPoint.y + 1);
					betterCoord3 = Coordinate(closestPoint.x - 1, closestPoint.y - 1);
				}
			} else if (closestPoint.x < targetRouter.x) {
				betterCoord1 = Coordinate(closestPoint.x + 1, closestPoint.y);
				if (closestPoint.y > targetRouter.y) {
					betterCoord2 = Coordinate(closestPoint.x + 1, closestPoint.y - 1);
					betterCoord3 = Coordinate(closestPoint.x, closestPoint.y - 1);
				} else if (closestPoint.y < targetRouter.y) {
					betterCoord2 = Coordinate(closestPoint.x + 1, closestPoint.y + 1);
					betterCoord3 = Coordinate(closestPoint.x, closestPoint.y + 1);
				} else if (closestPoint.y == targetRouter.y) {
					betterCoord2 = Coordinate(closestPoint.x + 1, closestPoint.y + 1);
					betterCoord3 = Coordinate(closestPoint.x + 1, closestPoint.y - 1);
				}
			} else if (closestPoint.x == targetRouter.x) {
				if (closestPoint.y > targetRouter.y) {
					betterCoord1 = Coordinate(closestPoint.x + 1, closestPoint.y - 1);
					betterCoord2 = Coordinate(closestPoint.x, closestPoint.y - 1);
					betterCoord3 = Coordinate(closestPoint.x - 1, closestPoint.y - 1);
				} else if (closestPoint.y < targetRouter.y) {
					betterCoord1 = Coordinate(closestPoint.x + 1, closestPoint.y + 1);
					betterCoord2 = Coordinate(closestPoint.x, closestPoint.y + 1);
					betterCoord3 = Coordinate(closestPoint.x - 1, closestPoint.y + 1);
				} else {
					canGetCloser = false;
				}
			}
			// Check if there is a wire at one of the 3 coordinates
			if (plan.isWired(betterCoord1) &&
				distance(betterCoord1, targetRouter) < distance(betterCoord1, closestPoint))
				closestPoint = betterCoord1;
			else if (plan.isWired(betterCoord2) &&
					 distance(betterCoord1, targetRouter) < distance(betterCoord1, closestPoint))
				closestPoint = betterCoord2;
			else if (plan.isWired(betterCoord3) &&
					 distance(betterCoord1, targetRouter) < distance(betterCoord1, closestPoint))
				closestPoint = betterCoord3;
			else canGetCloser = false;

		} while (canGetCloser);
	}

	return closestPoint;
}

void eraseFromVector(std::vector<Coordinate> &vector, const Coordinate &coord) {

	std::vector<Coordinate>::iterator it;

	it = std::find(vector.begin(), vector.end(), coord);
	if (it < vector.end()) {
		vector.erase(it);
	}

}

void eraseFromVector(std::vector<Coordinate> &vector, std::vector<Coordinate> &vectorToRemove) {

	for (auto it = vectorToRemove.begin(); it != vectorToRemove.end(); it++) {
		//std::cout << "Trying to remove " << *it << " from : " << std::endl << vector;
		eraseFromVector(vector, *it);

	}

}

std::ostream &operator<<(std::ostream &o, std::vector<Coordinate> &vector) {

	for (auto it = vector.begin(); it != vector.end(); it++) {
		o << *it << std::endl;
	}
	return o;
}