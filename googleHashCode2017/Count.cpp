#include"Count.h"
Count::Count() {
	start = GetTickCount();
	frequency = 0;
}

void Count::startTimer()
{
	start = GetTickCount();
}

void Count::stopTimer()
{
	end = GetTickCount();
	std::cout << "The "<< ++frequency <<"st Running Time is " << end - start << "ms" << std::endl;
	times.push_back(end - start);
}

double Count::getAverageRunningTime() {
	double sum = std::accumulate(std::begin(times), std::end(times), 0.0);
	std::cout << "The average of Running Time is " << sum / times.size() << "ms" << std::endl;
	if (times.size())
		return sum / times.size(); 
	else
		return 0;
}

int Count::getScore(int targetCell,int budget,
	int additionalCellsConnected2Backbone,int priceConnecting2Backbone,
	int priceRouter, int numberRouter) {
	//score = 1000 ¡Á t + (B ? (N ¡Á Pb +M ¡Á Pr))
	return 1000 * targetCell + (budget - (additionalCellsConnected2Backbone * priceConnecting2Backbone + priceRouter * numberRouter));
}