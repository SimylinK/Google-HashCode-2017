#include <iostream>
#include "Plan.h"
#include "Solver.h"
#include <fstream>

#include "Output.h"

int main() {
	Plan p("../inputData/opera.in");

	std::cout << "MaxBudget : " << p.getMaxBudget() << std::endl;
	std::cout << "Router cost " << p.getRouterCost() << std::endl;
	std::cout << "Wire cost " << p.getWireCost() << std::endl;
	solveProblem(p);

	std::cout << "money used : " << p.getSpentMoney() << std::endl;

	// To fill an output file (in notepadd++ you can dezoom to analyse big maps)
	std::ofstream file("../output/test.out", std::ios::out);
	if (file) {
		file << p << std::endl;
	}

	try {
		output(p, "../outputFile.out");
	}
	catch (std::exception e) {
		std::cout << e.what();
	}

	return 0;
}