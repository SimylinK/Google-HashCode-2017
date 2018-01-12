#include <iostream>
#include <fstream>

#include "../googleHashCode2017/Plan.h"
#include "../googleHashCode2017/Solver.h"
#include "../googleHashCode2017/Output.h"
#include "Referee.h"
#include "Count.h"

int main(int argc, char* argv[]) {

	if (argc == 3) {

		std::cout << "-----------time------------" << std::endl;
		Count ct;// use it in the begin
		std::string buffer = argv[1];
	//	ct.runAllExe(buffer);// the path of .exe
	//	ct.getAverageRunningTime();// get time
		std::cout << "-----------score------------" << std::endl;

		std::string outputFile = argv[2];

		try {
			Referee a(buffer, outputFile);
			int score = a.calculateScore();

			std::cout << "Score = " << score << std::endl;

		}
		catch (std::exception e) {
			std::cout << "This name of output file isn't valid. \n\nValid names of file are :\n../inputData/charleston_road.out\n../inputData/lets_go_higher.out\n../inputData/opera.out\n../inputData/rue_de_londres.out" << std::endl;
		}
	}
	else {
		std::cout << "Please provide as arguments the directory of executables and the output file in this order" << std::endl;
	}

	system("PAUSE");
	return 0;
}