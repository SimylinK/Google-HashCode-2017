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
	std::vector<Coordinate> firstRouters = placeRoutersIterative(p);

	std::cout << "--------------------Filling blanks--------------------" << std::endl;
	fillBlanks(p);

	std::cout << "--------------------Positioning wires--------------------" << std::endl;
	linkRouters(p);

}

/**
* Position all possible routers with maximum coverage area
* @param p: the plan to position the routers on
* @param spentMoney : the money used until now, will be updated during this method
*/
std::vector<Coordinate> placeRoutersIterative(Plan &p) {
	int maxReachableCells = (p.getRouterRange() * 2 + 1) * (p.getRouterRange() * 2 + 1);
	int routerCost = p.getRouterCost();
	int nbRouters = 0;

	for (int i = 0; i < p.getRows() && p.getSpentMoney() < p.getMaxBudget() - routerCost; i++) {
		for (int j = 0; j < p.getColumns() && p.getSpentMoney() < p.getMaxBudget() - routerCost; j++) {
			if (!p(i,j).isCovered() && p(i, j).getNumberOfCoverableCells() >= maxReachableCells) { // we can't cover the maxReachableCells if we're covered
				Coordinate c(i, j);
				p.addRouter(c);
				nbRouters++;
			}
		}
	}
	std::cout << "Positionned " << nbRouters << " routers that cover " << maxReachableCells << " cells" << std::endl;
	return p.getRouters(); // we haven't added any other router
}

void fillBlanks(Plan &p){
	int step = 1; // set this to 10 if you're debugging and want the function to go 10 times faster
	int maxBuget = p.getMaxBudget();
	int routerCost = p.getRouterCost();
	int maxReachableCells = (p.getRouterRange() * 2 + 1) * (p.getRouterRange() * 2 + 1);

	for (int reachableCells = maxReachableCells -1; reachableCells>0; reachableCells-=step) {
		int nbRouters = 0;
		if (reachableCells-step <= 0){
			reachableCells = step;
		}
		for (int i = 0; i < p.getRows() && p.getSpentMoney() < maxBuget - routerCost; i++) {
			for (int j = 0; j < p.getColumns() && p.getSpentMoney() < maxBuget - routerCost; j++) {
				if (p(i, j).getNumberOfCoverableCells() > reachableCells-step && p.getSpentMoney() < maxBuget - routerCost) {
					Coordinate c(i, j);
//					auto routersAndBackbone = p.getRouters();
//					routersAndBackbone.push_back(p.getBackbone());
//					Coordinate bestRouter = argDistMin(c, routersAndBackbone);
//					Coordinate placeToLink = followWire(p, bestRouter, c);
//					p.link(placeToLink, c);
					p.addRouter(c);
					nbRouters++;
				}
			}
		}
		if(step == 1){
			std::cout << "Positionned " << nbRouters << " routers covering " << reachableCells << " cells" << std::endl;
		} else {
			std::cout << "Positionned " << nbRouters << " routers covering between [" << reachableCells << ", " << reachableCells-step+1  << "] cells" << std::endl;

		}
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
	auto it = std::find(vector.begin(), vector.end(), coord);
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

/**
 *
 * @param p : the plan to work on
 */
void linkRouters(Plan &p) {
	int routersRemoved = 0;
	while (linkStratTwo(p)) { // while we use too much money
		std::cout << "Routers removed " << ++routersRemoved << std::endl;
		p.removeRouters(1, p.getWires().size()-1); // remove the last router since it's the one which covers the less space, and remove all the wires
	}
}

/**
 *
 * @param p : the plan to work on
 * @return a boolean telling us if we spent too much money or not
 */
bool linkStratTwo(Plan &p) {
	auto routers = p.getRouters();
	int maxBuget =  p.getMaxBudget();
	std::vector<Coordinate> wiredGroup;
	Coordinate minToBackbone = argDistMin(p.getBackbone(), routers); // get the router closer to the backbone, then link it
	p.link(p.getBackbone(), minToBackbone);
	wiredGroup.push_back(p.getBackbone());
	wiredGroup.push_back(minToBackbone);
	eraseFromVector(routers, minToBackbone); // no need to link this router anymore

	std::pair<Coordinate, Coordinate> bestPair;
	Coordinate placeToLink;
	while(routers.size() > 0 && p.getSpentMoney() < maxBuget){
		bestPair = minTwoGroups(wiredGroup,routers); // find the min between the group
		placeToLink = followWire(p, bestPair.first, bestPair.second);
		p.link(placeToLink, bestPair.second);
		wiredGroup.push_back(bestPair.second);
		eraseFromVector(routers, bestPair.second);
	}
	bool spentMore = p.getSpentMoney() > p.getMaxBudget() || routers.size() > 0; // if we spent too much money of if there is a router still not linked
	return spentMore;
}

/**
 *
 * @param alreadyLinked : a vector of already linked routers
 * @param toLink : a vector of routers to link
 * @return a pair of coordinates with first the linked router and second the router to link
 */
std::pair<Coordinate, Coordinate> minTwoGroups(std::vector<Coordinate> alreadyLinked, std::vector<Coordinate> toLink){
	std::pair<Coordinate, Coordinate> bestPair;
	Coordinate bestRouter = argDistMin(alreadyLinked[0], toLink);
	int bestDistance = distance(alreadyLinked[0], bestRouter);
	bestPair.first =  alreadyLinked[0];
	bestPair.second = bestRouter;

	Coordinate tryRouter;
	for (auto& linkedRouter : alreadyLinked){
		tryRouter = argDistMin(linkedRouter, toLink);
		if (distance(tryRouter, linkedRouter) < bestDistance){
			bestPair.first = linkedRouter;
			bestPair.second = tryRouter;
		}
	}
	return bestPair;
}