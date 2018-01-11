#include"Count.h"

/**
* choose use which way 0 or 1
* default way =0 use clock() unit is s 
* way =1 or other numbers use time(&argument) unit is s
* attention : in linux maybe clock() doesn't work always return 0 , so you can use the other way
* @param w: 0 or 1 
* @return null
*/
Count::Count(int w):way(w)
{
}

/**
* start the timer
* @param null
* @return null
*/
void Count::startTimer()
{
	if (way == 0)
		dstart = clock();
	else
		time(&tstart);
}

/**
* stop the timer and save the running time
* @param null
* @return null
*/
void Count::stopTimer()
{
	double cost;
	if (way == 0) {
		dend = clock();
		cost = (dend - dstart)/1000;
	}
	else {
		time(&tend);
		cost = difftime(tend, tstart);
	}
	
	std::cout << "The "<< ++frequency <<"st Running Time is " << cost << "s" << std::endl;
	times.push_back(cost);
}

/**
* get the average value of these running times
* @param null
* @return the average value
*/
double Count::getAverageRunningTime() {
	double sum = std::accumulate(std::begin(times), std::end(times), 0.0);
	std::cout << "The average of Running Time is " << sum / times.size() << "s" << std::endl;
	if (times.size())
		return sum / times.size(); 
	else
		return 0;
}

/**
* get all paths of files in the target folder
* @param path : the path of target folder
* @param files : the path of each file in the target folder
* @return null
*/
void Count::getFiles(std::string path, std::vector<std::string>& files)
{ 
	long   hFile = 0;//File handle
	struct _finddata_t fileInfo;//information of file 
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileInfo)) != -1)
	{
		do
		{
			std::cout << p.assign(path).append("/").append(fileInfo.name) << std::endl;
			// If the directory, iteration
			// If not, add to the list
			if ((fileInfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
					getFiles(p.assign(path).append("/").append(fileInfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("/").append(fileInfo.name));
			}
		} while (_findnext(hFile, &fileInfo) == 0);
		_findclose(hFile);
	}
	else
	{
		std::cout << "input the wrong path" <<std::endl;
		exit(0);
	}
}

/**
* run all exe
* @param path : the path of target folder
* @return null
*/
void Count::runAllExe(const std::string& path){
	std::string filePath = path;
	std::vector<std::string> fileNames;
	//getAllFileName 
	getFiles(filePath, fileNames);
	std::cout << "find all .exe:" << std::endl;
	for (std::string e : fileNames) {
		std::cout << e << std::endl;
	}
	std::cout << std::endl;
	std::cout <<"Timer starts:"<< std::endl;
	for (std::string e : fileNames) {
		startTimer();
		if (system(e.data()) != 0) {
			std::cout << "can not run the *.exe" << std::endl;
			exit(0);
		}
		stopTimer();
	}
}