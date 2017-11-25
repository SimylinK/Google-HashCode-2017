#pragma once
#ifdef WIN32
#include <windows.h>
#else 
#include <unistd.h>
#endif 
#include<time.h>
#include<iostream>
#include<vector>
#include<numeric>
#include<shellapi.h>
#include<string>
#include "io.h"

#define __T(x) L ## x
#define _T(x) __T(x)

class Count {
public:
	Count(int w = 0);//to choice use which way 0 or 1;
	void startTimer();//to start the timer
	void stopTimer();//to stop the timer and save the running time
	double getAverageRunningTime();//to get the average value of these running times
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
