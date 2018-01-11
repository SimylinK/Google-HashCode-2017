#include <iostream>
#include <fstream>

#include "../googleHashCode2017/Plan.h"
#include "../googleHashCode2017/Solver.h"
#include "../googleHashCode2017/Output.h"
#include "Referee.h"

#include "Count.h"

int main(int argc, char* argv[]) {

	std::cout << argv[0] << std::endl;
	Count ct;// use it in the begin
			 //std::string buffer = "./runExe";
	//ct.runAllExe(buffer);// the path of .exe
	//getAverageRunningTime();// get time
	std::cout << "coucou" << std::endl;

	std::string e = "C:/Users/Mathilde/Desktop/Google_Hashcode_2017/googleHashCode2017_Arbitre/runExe";
	if (system(e.data()) != 0) {
		std::cout << "can not run the *.exe" << std::endl;
		exit(0);
	}
	std::cout << "coucou" << std::endl;
	/*
	std::cout << "coucou = " << std::endl;

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


*/
	system("PAUSE");
	
	return 0;
}