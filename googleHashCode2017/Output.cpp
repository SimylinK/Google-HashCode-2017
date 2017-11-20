#include "Output.h"
#include <iostream>
#include <fstream>
#include <exception>

//create output file
void output(Plan plan, std::string outputPath) {

	std::ofstream outputFile;
	outputFile.open(outputPath, std::ofstream::out);

	if (outputFile.bad()) {
		throw new std::exception();
	}

	outputFile << plan.getWires().size() << std::endl; //Number of cells connected to the backbone

	for (auto &wire : plan.getWires()) {
		outputFile << wire.x << " " << wire.y << std::endl; //Coordinates of cells connected to the backbone
	}
	outputFile << plan.getRouters().size() << std::endl; //Number of routers

	for (auto &router : plan.getRouters()) {
		outputFile << router.x << " " << router.y << std::endl; //Coordinates of routers
	}

	outputFile.close();
}