#include <iostream>
#include "Plan.h"
#include "Solver.h"

int main() {
	Plan p("../inputData/test.in");
	std::cout << p;
	//placeRoutersIterative(p);
	//std::cout << p;

	Coordinate router1 = Coordinate(3,2);
	Coordinate router2 = Coordinate(4,5);
	Coordinate router3 = Coordinate(8,1);
	p.addRouter(router1);
	p.addRouter(router2);
	p.addRouter(router3);

	int money = 0;
	std::vector<Coordinate> listBarycentres{ Coordinate(1,1) };
	std::vector<Coordinate> listRouters{Coordinate(3,2), Coordinate(4,5), Coordinate(8,1)};

	sectorLink(p, listBarycentres, listRouters, money);

	std::cout << p;
	std::cout << "money : " << money << std::endl;
	return 0;
}