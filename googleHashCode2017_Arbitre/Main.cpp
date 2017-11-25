#include <iostream>
#include <fstream>

#include "../googleHashCode2017/Plan.h"
#include "../googleHashCode2017/Solver.h"
#include "../googleHashCode2017/Output.h"
#include "Referee.h"

int main() {

	Referee a("../outputFile.out", "../inputData/test.in");

	std::cout << "Score = " << a.calculateScore() << std::endl;

	return 0;
}