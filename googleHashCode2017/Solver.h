#ifndef GOOGLEHASHCODE2017_SOLVER_H
#define GOOGLEHASHCODE2017_SOLVER_H

#include "Plan.h"

void solveProblem(Plan &p);
void placeRoutersIterative(Plan &p);
std::list<std::pair<int, int>> gridWiring(std::list<std::pair<int, int>> &listSectorRouters, Plan &p);
Coordinate computeBarycentre(const std::vector<Coordinate> &listCoord);
Coordinate argDistMin(const Coordinate &point, const std::vector<Coordinate> &listCoord);

void sectorLink(Plan &plan, const std::vector<Coordinate> &listBarycentres, std::vector<Coordinate> &initialListRouters,
				std::vector<Coordinate> &listConnectedRouters, bool reversedMode);

void recursiveLink(Plan &plan, const Coordinate &router, const std::vector<Coordinate> &listRouters,
				   const Coordinate &barycentre, std::vector<Coordinate> &listConnectedRouters, bool reversedMode);
Coordinate followWire(Plan &plan, const Coordinate &startRouter, const Coordinate &targetRouter);


void eraseFromVector(std::vector<Coordinate> &vector, const Coordinate &coord);
void eraseFromVector(std::vector<Coordinate> &vector, std::vector<Coordinate> &vectorToRemove);

std::ostream &operator<<(std::ostream &o, std::vector<Coordinate> &vector);

#endif //GOOGLEHASHCODE2017_SOLVER_H
