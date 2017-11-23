#include <iostream>
#include "Plan.h"
#include "Solver.h"
#include <fstream>

int main() {
	Plan p("../inputData/test.in");

	//std::cout << p;

	int money = 0;

	std::cout <<"MaxBudget : "<< p.getMaxBudget() << std::endl;
	std::cout <<"Router cost "<< p.getRouterCost() << std::endl;
	std::cout <<"Wire cost " <<p.getWireCost() << std::endl;
	//placeRoutersIterative(p, 10);
	//sectorLink(p, listBarycentres, listRouters, money);
	money = solveProblem(p);

	//std::cout << p;
	std::cout << "money used : " << money << std::endl;

	// To fill an output file (in notepadd++ you can dezoom to analyse big maps)
	std::ofstream file("../output/test.out", std::ios::out);
	if (file) {
		file << p << std::endl;
	}

	return 0;
}