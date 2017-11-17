#include<windows.h> 
#include<iostream>
#include<vector>
#include<numeric>
/* 
exemple of use

Count ct;// use it in the begin

ct.startTimer();
Sleep(1000);//programmer l'algori 1er fois
ct.stopTimer();

ct.startTimer();
Sleep(1500);//programmer l'algori 2eme fois
ct.stopTimer();

for (int i = 0; i < 2; i++)
{
	ct.startTimer();
	Sleep(1000);//programmer l'agori  3eme et 4eme fois
	ct.stopTimer();
}

ct.getAverageRunningTime();// use it it the end
*/

class Count {
public:
	Count();
	void startTimer();//to start the timer
	void stopTimer();//to stop the timer and save the running time
	double getAverageRunningTime();//to get the average value of these running times
	int getScore(int targetCell, int budget,
		int additionalCellsConnected2Backbone, int priceConnecting2Backbone,
		int priceRouter, int numberRouter);//to get score of algorithm
private:
	double start;//the time of start
	double  end ;//the time of end
	int frequency;// the frequency of running
	std::vector<double> times;//save every running time
};

