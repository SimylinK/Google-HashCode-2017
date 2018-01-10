#include <iostream>
#include <fstream>

#include "../googleHashCode2017/Plan.h"
#include "../googleHashCode2017/Solver.h"
#include "../googleHashCode2017/Output.h"
#include "Referee.h"

int main() {
	/*
	Count ct;// use it in the begin
	ct.runAllExe(".\\runExe");// the path of .exe
	ct.getAverageRunningTime();// get time
	*/

	std::string map = "charleston_road";
	std::string inputFile = "../inputData/" + map + ".in";

	std::string repertoryName = "test";

	try {
		Referee a(repertoryName, "../" + map + ".out");
		int score = a.calculateScore();

		std::cout << "Score = " << score << std::endl;
	}
	catch (std::exception e) {
		std::cout << "This name of output file isn't valid. \n\nValid names of file are :\n../inputData/charleston_road.in\n../inputData/lets_go_higher.in\n../inputData/opera.in\n../inputData/rue_de_londres.in" << std::endl;
	}

//	std::cout << a << std::endl;

	return 0;
}