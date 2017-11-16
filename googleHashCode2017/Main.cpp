#include <iostream>
#include "Plan.h"
#include "Solver.h"

int main() {
	Plan p("../inputData/test.in");
	std::cout << p;
	placeRoutersIterative(p);
	std::cout << p;

	return 0;
}