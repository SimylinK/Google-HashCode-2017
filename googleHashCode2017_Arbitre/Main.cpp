#include <iostream>
#include <fstream>

#include "../googleHashCode2017/Plan.h"
#include "../googleHashCode2017/Solver.h"
#include "../googleHashCode2017/Output.h"
#include "Referee.h"
#include "Count.h"

int main() {

	std::cout << "-----------time------------" << std::endl;
	Count ct;// use it in the begin
	std::string buffer = "..\\runExe";
	ct.runAllExe(buffer);// the path of .exe
	ct.getAverageRunningTime();// get time
	std::cout << "-----------score------------" << std::endl;

	std::string map = "charleston_road";
	std::string inputFile = "../inputData/" + map + ".in";

	std::string repertoryName = "test";

	try {
		Referee a(repertoryName, "../" + map + ".out");
		int score = a.calculateScore();

		std::cout << "Score = " << score << std::endl;

	}
	catch (std::exception e) {
		std::cout << "This name of output file isn't valid. \n\nValid names of file are :\n../inputData/charleston_road.out\n../inputData/lets_go_higher.out\n../inputData/opera.out\n../inputData/rue_de_londres.out" << std::endl;
	}
	return 0;
}