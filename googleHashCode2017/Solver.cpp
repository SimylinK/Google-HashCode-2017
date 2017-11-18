#include <cmath>
#include "Solver.h"

void placeRoutersIterative(Plan &p) {
	int maxReachableCells = (p.getRouterRange()*2 + 1)*(p.getRouterRange()*2 + 1);

	for(int i=0; i<p.getRows(); i++){
		for (int j=0; j<p.getColumns(); j++) {
			if (p.coverableCells(Coordinate(i, j)).size() == maxReachableCells) {
				Coordinate c(i,j);
				p.addRouter(c);
			}
		}
	}
}


/**
* Find the barycentre of a list of Coordinate
* @param listCoord: the list of Coordinate from which to find the barycentre
* @return the barycentre of the list
*/
Coordinate& computeBarycentre(const std::vector<Coordinate> &listCoord) {
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
Coordinate& argDistMin(const Coordinate &point, const std::vector<Coordinate> &listCoord) {
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
* @param plan: the plan to work with
* @param listBarycentres: the barycentres where we connect the routers
* @param initialListRouters: list of the rooters to connect to the network
* @param money: the money used until now, will be upadta during this method
*/
void sectorLink(Plan &plan, const std::vector<Coordinate> &listBarycentres, const std::vector<Coordinate> &initialListRouters, int &money) {
	Coordinate barycentre = computeBarycentre(initialListRouters);
	Coordinate closestBarycentre = argDistMin(barycentre, listBarycentres);
	plan.link(closestBarycentre, barycentre, money);

	std::vector<Coordinate> routersToConnect = initialListRouters;

	while (routersToConnect.size() > 0) {
		std::vector<Coordinate> listConnectedRouters;
		recursiveLink(plan, routersToConnect[0], initialListRouters, money, barycentre, listConnectedRouters);

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
void recursiveLink(Plan &plan, const Coordinate &router, const std::vector<Coordinate> &listRouters, int &money, const Coordinate &barycentre, std::vector<Coordinate> &listConnectedRouters) {
	listConnectedRouters.push_back(router);
	std::vector<Coordinate> listCoordinatesForLink = listRouters;

	// Remove the router from  listCoordinatesForLink
	eraseFromVector(listCoordinatesForLink, router);

	listCoordinatesForLink.push_back(barycentre);

	// Find closest point to link
	Coordinate closestLinkablePoint = argDistMin(router, listCoordinatesForLink);

	if (!plan.isWired(Coordinate(closestLinkablePoint.x, closestLinkablePoint.y))) {
		recursiveLink(plan, closestLinkablePoint, listRouters, money, barycentre, listConnectedRouters);
	}

	closestLinkablePoint = followWire(plan, closestLinkablePoint, router);

	plan.link(closestLinkablePoint, router, money);
}

/**
* Find the closest connected point from a router to another
* @param startRouter: a router already connected
* @param targetRouter: the router to get closer from
* @return the coordinate of the connected point closest to the target
*/
Coordinate& followWire(Plan &plan, const Coordinate &startRouter, const Coordinate &targetRouter) {
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
		if (plan.isWired(Coordinate(betterCoord1.x, betterCoord1.y))) closestPoint = betterCoord1;
		else if (plan.isWired(Coordinate(betterCoord2.x, betterCoord2.y))) closestPoint = betterCoord2;
		else if (plan.isWired(Coordinate(betterCoord2.x, betterCoord2.y))) closestPoint = betterCoord3;
		else canGetCloser = false;

	} while (canGetCloser);

	return closestPoint;
}


void eraseFromVector(std::vector<Coordinate> &vector, const Coordinate &coord) {
	std::vector<Coordinate>::iterator it;
	it = std::find(vector.begin(), vector.end(), coord);
	vector.erase(it);
}