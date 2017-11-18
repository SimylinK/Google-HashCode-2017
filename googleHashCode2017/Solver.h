#ifndef GOOGLEHASHCODE2017_SOLVER_H
#define GOOGLEHASHCODE2017_SOLVER_H

#include "Plan.h"

void placeRoutersIterative(Plan &p);

// For the first solver solution
Coordinate& computeBarycentre(const std::vector<Coordinate> &listCoord);
Coordinate& argDistMin(const Coordinate &point, const std::vector<Coordinate> &listCoord);

void sectorLink(Plan &plan, const std::vector<Coordinate> &listBarycentres, const std::vector<Coordinate> &initialListRouters, int &money);
void recursiveLink(Plan &plan, const Coordinate &router, const std::vector<Coordinate> &listRouters, int &money, const Coordinate &barycentre, std::vector<Coordinate> &listConnectedRouters);
Coordinate& followWire(Plan &plan, const Coordinate &startRouter, const Coordinate &targetRouter);
void eraseFromVector(std::vector<Coordinate> &vector, const Coordinate &coord);

#endif //GOOGLEHASHCODE2017_SOLVER_H
