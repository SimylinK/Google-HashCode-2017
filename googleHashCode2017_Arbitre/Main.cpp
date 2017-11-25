#include <iostream>
#include <fstream>

#include "../googleHashCode2017/Plan.h"
#include "../googleHashCode2017/Solver.h"
#include "../googleHashCode2017/Output.h"
#include "Referee.h"
#include "Count.h"

int main() {

	Referee a("../outputFile.out", "../inputData/test.in");

	std::cout << "Score = " << a.calculateScore() << std::endl;


	Count ct;// use it in the begin
	ct.runAllExe(".\\runExe");// the folder of exe
	ct.getAverageRunningTime();// use it it the end
	return 0;
}