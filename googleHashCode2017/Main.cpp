#include <iostream>
#include "Plan.h"
#include "Solver.h"
#include <fstream>

#include "Output.h"

int main() {
	Plan p("../inputData/charleston_road.in");

	std::cout << p;

	int money = 0;

	std::cout <<"MaxBudget : "<< p.getMaxBudget() << std::endl;
	std::cout <<"Router cost "<< p.getRouterCost() << std::endl;
	std::cout <<"Wire cost " <<p.getWireCost() << std::endl;
	//placeRoutersIterative(p, 10);
	//sectorLink(p, listBarycentres, listRouters, money);
	money = solveProblem(p, 9);

	std::cout << p;
	std::cout << "money used : " << money << std::endl;

	// To fill an output file (in notepadd++ you can dezoom to analyse big maps)
	/*std::ofstream file("../output/test.out", std::ios::out);
	if (file) {
		file << p << std::endl;
	}*/

	try {
		output(p, "../charleston_road.out");
	}
	catch (std::exception e) {
		std::cout << e.what();
	}

	return 0;
}