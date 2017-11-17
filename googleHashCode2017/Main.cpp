#include <iostream>
#include "Plan.h"
#include "Solver.h"

#include "Output.h"

int main() {
	Plan p("../inputData/test.in");
	std::cout << p;
	placeRoutersIterative(p);
	std::cout << p;

	try {
		output(p, "../outputFile.out");
	}
	catch (std::exception e) {
		std::cout << e.what();
	}

	return 0;
}