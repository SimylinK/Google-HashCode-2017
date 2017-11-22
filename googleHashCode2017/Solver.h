#ifndef GOOGLEHASHCODE2017_SOLVER_H
#define GOOGLEHASHCODE2017_SOLVER_H

#include "Plan.h"


void placeRoutersIterative(Plan &p, std::vector<Coordinate>& sectorRouters, int n, int& spentMoney);

// For the first solver solution
Coordinate computeBarycentre(const std::vector<Coordinate> &listCoord);
Coordinate argDistMin(const Coordinate &point, const std::vector<Coordinate> &listCoord);

void sectorLink(Plan &plan, const std::vector<Coordinate> &listBarycentres,  std::vector<Coordinate> &initialListRouters,
	int &money,  std::vector<Coordinate> & listConnectedRouters, bool reversedMode);




void recursiveLink(Plan &plan, const Coordinate &router, const std::vector<Coordinate> &listRouters, int &money, 
					const Coordinate &barycentre, std::vector<Coordinate> &listConnectedRouters, bool reversedMode);
Coordinate followWire(Plan &plan, const Coordinate &startRouter, const Coordinate &targetRouter);


void eraseFromVector(std::vector<Coordinate> &vector, const Coordinate &coord);
void eraseFromVector(std::vector<Coordinate> &vector, std::vector<Coordinate> &vectorToRemove);
int solveProblem(Plan &p, int nbRouterSector);

std::ostream& operator<<(std::ostream& o, std::vector<Coordinate> &vector);

#endif //GOOGLEHASHCODE2017_SOLVER_H
