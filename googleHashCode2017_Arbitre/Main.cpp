#include <iostream>
#include <fstream>

#include "../googleHashCode2017/Plan.h"
#include "../googleHashCode2017/Solver.h"
#include "../googleHashCode2017/Output.h"
#include "Referee.h"

int main() {

	std::string inputFile = "../inputData/opera.in";
	

	Plan p(inputFile);
	solveProblem(p, 9);


	

	try {
		output(p, "../outputFile.out");
	}
	catch (std::exception e) {
		std::cout << e.what();
	}

//	Referee a("../outputFile.out", inputFile);

//	std::cout << a;

//	std::cout << "Score = " << a.calculateScore() << std::endl;

	return 0;
}