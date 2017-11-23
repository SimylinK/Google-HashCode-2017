#include <cmath>
#include "Solver.h"
#include <map>
#include <iostream>
#include <math.h> 

/**
* Position and link a network of routers on a plan to maximise the number of covered cells
* @param p: the plan to position the routers on
* @param nbRouterSector: the maximum number of routers in a sector
* @return the money spent
*/
int solveProblem(Plan &p){

	int spentMoney = 0;

	std::cout << "Positioning routers" << std::endl;
	placeRoutersIterative(p, spentMoney);

	fillGrid(p);
	std::cout << "Positioning wires" << std::endl;

	//The list of sectors is initialized with the sector containing the backbone
	std::list<std::pair<int,int>> listSectorRouters{ std::pair<int,int>(p.getBackbone().x / p.getGridCell_heigth(),
																p.getBackbone().y / p.getGridCell_width()) };

	while(listSectorRouters.size() != 0) {
		listSectorRouters = gridWiring(listSectorRouters, p, spentMoney);
	}
	return spentMoney;
}


/**
* Wire a list of router sectors and return the list of neighboring router sectors
* @param listSectorRouters: the list of pair of indices (i,j) corresponding to the sectors to wire
* @param p: the plan where the routers are
* @param spentMoney : the money used until now, will be updated during this method
* @return the list of neighboring router sector, not wired yet
*/
std::list<std::pair<int, int>> gridWiring(std::list<std::pair<int,int>>& listSectorRouters, Plan &p,int &spentMoney) {
	int lastNbWireSector; //The number of wires that was required to link a sector
	int lastSpentMoney;
	int nbFirstWires;
	std::vector<Coordinate> listBarycentres;
	std::vector<Coordinate> sectorRouters;
	Coordinate currentBary;
	Coordinate closestBackboneRouter;
	std::list<std::pair<int, int>> listNeighbSectorRouters;
	bool sectorIsPostionned;

	for (auto coord_sectorRouters : listSectorRouters) {
		sectorRouters = p.grid(coord_sectorRouters);
		sectorIsPostionned = false;
		do {
			if (sectorRouters.size() > 0) {

				lastSpentMoney = spentMoney;

				std::vector<Coordinate> listConnectedRouters;

				currentBary = computeBarycentre(sectorRouters);
				closestBackboneRouter = argDistMin(p.getBackbone(), sectorRouters);
				listBarycentres.push_back(currentBary);

				if (distance(closestBackboneRouter, p.getBackbone()) < distance(currentBary, p.getBackbone())) {
					p.link(p.getBackbone(), closestBackboneRouter, spentMoney);
					recursiveLink(p, closestBackboneRouter, sectorRouters, spentMoney, currentBary, listConnectedRouters, true);
				}
				else {
					p.link(p.getBackbone(), currentBary, spentMoney);
				}

				sectorLink(p, listBarycentres, sectorRouters, spentMoney, listConnectedRouters, false);
				lastNbWireSector = (spentMoney - lastSpentMoney) / p.getWireCost();

				if (spentMoney > p.getMaxBudget()) {
					p.removeRouters(static_cast<int>(sectorRouters.size()), lastNbWireSector);
					spentMoney -= static_cast<int>(sectorRouters.size())*p.getRouterCost() + (lastNbWireSector)*p.getWireCost();
					sectorRouters.pop_back(); //If there's not enough money to position all the routers of the sector
											  //the algorithm tries again with one less router
				}
				else {
					sectorIsPostionned = true;
					for (auto neighRouterSector : p.grid.getNeighbors(coord_sectorRouters)){
						if (!p.isGridWired(neighRouterSector)) {
							listNeighbSectorRouters.push_back(neighRouterSector);
						}
					}
				}
			}
			//If no router can be wired (because there's not enough money)
			//or if the routers have been wired, the algorithm stops
		} while (sectorRouters.size() > 0 && !sectorIsPostionned);
	}

	return listNeighbSectorRouters;
}


/**
* Position all possible routers with maximum coverage area
* @param p: the plan to position the routers on
* @param spentMoney : the money used until now, will be updated during this method
*/
void placeRoutersIterative(Plan &p, int& spentMoney) {
	int maxReachableCells = (p.getRouterRange()*2 + 1)*(p.getRouterRange()*2 + 1);
	int numberPlacedRouters = 0;
	int i= 0;
	int j;
	while (i < p.getRows()){
		j = 0;
		while (j < p.getColumns()){
			if (p.coverableCells(Coordinate(i, j)).size() == maxReachableCells) {
				Coordinate c(i,j);
				p.addRouter(c);
				numberPlacedRouters++;		
			}			
			j++;
		}
		i++;
	}
	spentMoney += numberPlacedRouters*p.getRouterCost();
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

	return  Coordinate(baryX, baryY);
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
* @param plan: the plan to work with
* @param listBarycentres: the barycentres where we connect the routers
* @param initialListRouters: list of the rooters to connect to the network
* @param money: the money used until now, will be upadta during this method
*/

void sectorLink(Plan &plan, const std::vector<Coordinate> &listBarycentres, std::vector<Coordinate> &initialListRouters,
														int &money, std::vector<Coordinate> & listConnectedRouters, 
														bool reversedMode) {

	Coordinate barycentre = computeBarycentre(initialListRouters);
	Coordinate closestBarycentre = argDistMin(barycentre, listBarycentres);
	plan.link(closestBarycentre, barycentre, money);

	std::vector<Coordinate> routersToConnect = initialListRouters;

	while (routersToConnect.size() > 0) {

		recursiveLink(plan, routersToConnect[0], initialListRouters, money, barycentre, listConnectedRouters,reversedMode);

		for (Coordinate router : listConnectedRouters)
		{
			eraseFromVector(routersToConnect, router);
		}
		std::vector<Coordinate> listConnectedRouters;
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
void recursiveLink(Plan &plan, const Coordinate &router, const std::vector<Coordinate> &listRouters, int &money, 
									const Coordinate &barycentre, std::vector<Coordinate> &listConnectedRouters, bool reversedMode = false) {

	listConnectedRouters.push_back(router);
	std::vector<Coordinate> listCoordinatesForLink = listRouters;

	// Remove the router from  listCoordinatesForLink
	eraseFromVector(listCoordinatesForLink, router);
	//eraseFromVector(listCoordinatesForLink, listConnectedRouters);

	listCoordinatesForLink.push_back(barycentre);

	// Find closest point to link
	Coordinate closestLinkablePoint = argDistMin(router, listCoordinatesForLink);

	if (reversedMode == true) {
		closestLinkablePoint = followWire(plan, closestLinkablePoint, router);
		plan.link(router, closestLinkablePoint,money);
	}

	if ((!plan.isWired(closestLinkablePoint) || reversedMode == true) && router != barycentre) {
		recursiveLink(plan, closestLinkablePoint, listCoordinatesForLink, money, barycentre, listConnectedRouters, reversedMode);
	}


	if (reversedMode == false) {

		closestLinkablePoint = followWire(plan, closestLinkablePoint, router);
		plan.link(closestLinkablePoint, router, money);
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
			if (plan.isWired(betterCoord1) && distance(betterCoord1, targetRouter) < distance(betterCoord1, closestPoint)) closestPoint = betterCoord1;
			else if (plan.isWired(betterCoord2) && distance(betterCoord1, targetRouter) < distance(betterCoord1, closestPoint)) closestPoint = betterCoord2;
			else if (plan.isWired(betterCoord3) && distance(betterCoord1, targetRouter) < distance(betterCoord1, closestPoint)) closestPoint = betterCoord3;
			else canGetCloser = false;

		} while (canGetCloser);
	}

	return closestPoint;
}



/**
* Assign each router to a grid cell 
* @param plan: the plan to work with
* @param grid : a map of pairs linking a grid cell of a grid cell with the vector of routers assigned to that cell
*/
void fillGrid(Plan & plan) {

	std::vector<Coordinate> allRouters =plan.getRouters();



	for (auto router : allRouters) {
		plan.grid(std::pair<int, int>(router.x / plan.getGridCell_heigth(), router.y / plan.getGridCell_width())).push_back(router);
	}

}


void eraseFromVector(std::vector<Coordinate> &vector, const Coordinate &coord) {

	std::vector<Coordinate>::iterator it;
	
	it = std::find(vector.begin(), vector.end(), coord);
	if (it < vector.end()) {
		vector.erase(it);
	}

}

void eraseFromVector(std::vector<Coordinate> &vector, std::vector<Coordinate> & vectorToRemove) {

	for (auto it = vectorToRemove.begin(); it != vectorToRemove.end(); it++) {
		//std::cout << "Trying to remove " << *it << " from : " << std::endl << vector;
		eraseFromVector(vector, *it);

	}

}

std::ostream& operator<<(std::ostream& o, std::vector<Coordinate> &vector) {

	for (auto it = vector.begin(); it != vector.end(); it++) {
		o << *it << std::endl;
	}
	return o;
}