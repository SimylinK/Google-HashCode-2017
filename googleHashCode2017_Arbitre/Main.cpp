#include <iostream>
#include <fstream>

#include "../googleHashCode2017/Plan.h"
#include "../googleHashCode2017/Solver.h"
#include "../googleHashCode2017/Output.h"
#include "Referee.h"

int main() {

	std::string map = "charleston_road";
	std::string inputFile = "../inputData/" + map + ".in";
	

	Plan p(inputFile);

	Referee a("../" + map + ".out", inputFile);

//	std::cout << a << std::endl;

	std::cout << "Score = " << a.calculateScore() << std::endl;

	return 0;
}