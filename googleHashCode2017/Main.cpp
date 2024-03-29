#include <iostream>
#include "Plan.h"
#include "Solver.h"
#include <fstream>

#include "Output.h"


int main(int argc, char *argv[]) {

    if(argc == 2) {
        Plan p(argv[1]);

        std::cout << "MaxBudget : " << p.getMaxBudget() << std::endl;
        std::cout << "Router cost : " << p.getRouterCost() << std::endl;
        std::cout << "Wire cost : " << p.getWireCost() << std::endl;
        solveProblem(p);

        std::cout << "money used : " << p.getSpentMoney() << std::endl;
        std::cout << "percentage of cells covered : " << p.percentageCovered() << std::endl;
			
        try {
			std::string input(argv[1]);
			std::string outputFile = input.substr(0, input.size() - 2);
			outputFile.append("out");
            output(p, outputFile);
        }
        catch (std::exception e) {
            std::cout << e.what();
        }
    }
	return 0;
}