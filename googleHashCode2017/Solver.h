#ifndef GOOGLEHASHCODE2017_SOLVER_H
#define GOOGLEHASHCODE2017_SOLVER_H

#include "Plan.h"

void solveProblem(Plan &p);
std::vector<Coordinate> placeRoutersIterative(Plan &p);
void fillBlanks(Plan &p);
Coordinate argDistMin(const Coordinate &point, const std::vector<Coordinate> &listCoord);
Coordinate followWire(Plan &plan, const Coordinate &startRouter, const Coordinate &targetRouter);
void eraseFromVector(std::vector<Coordinate> &vector, const Coordinate &coord);
void eraseFromVector(std::vector<Coordinate> &vector, std::vector<Coordinate> &vectorToRemove);


void linkRouters(Plan &p);
bool linkStratTwo(Plan &p);

std::pair<Coordinate, Coordinate> minTwoGroups(std::vector<Coordinate> alreadyLinked, std::vector<Coordinate> toLink);


std::ostream &operator<<(std::ostream &o, std::vector<Coordinate> &vector);

#endif //GOOGLEHASHCODE2017_SOLVER_H
