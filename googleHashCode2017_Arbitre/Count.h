#pragma once
#include <windows.h>
#include<time.h>
#include<iostream>
#include<vector>
#include<numeric>
#include<shellapi.h>
#include<string>
#include "io.h"

class Count {
public:
	Count(int w = 0);//choose to use which way 0 or 1
	void startTimer();//start the timer
	void stopTimer();//stop the timer and save the running time
	double getAverageRunningTime();//get the average value of these running times
	void runAllExe(const std::string& path);//run all exe
	void Count::getFiles(std::string path, std::vector<std::string>& files);//get all filename in the folder
private:
	/*
	default way =0 use clock() unit is s 
	way =1 or other numbers use time(&argument) unit is s
	attention : in linux maybe clock() doesn't work always return 0 , so you can use the other way
	*/
	int way;
	time_t tstart;//the time of start 
	time_t  tend ;//the time of end
	double dstart;//the time of start
	double  dend ;//the time of end

	int frequency;// the frequency of running
	std::vector<double> times;//save every running time
};

