#include"Count.h"

Count::Count(int w):way(w)
{
}

void Count::startTimer()
{
	if (way == 0)
		dstart = clock();
	else
		time(&tstart);
}

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

double Count::getAverageRunningTime() {
	double sum = std::accumulate(std::begin(times), std::end(times), 0.0);
	std::cout << "The average of Running Time is " << sum / times.size() << "s" << std::endl;
	if (times.size())
		return sum / times.size(); 
	else
		return 0;
}

void Count::getFiles(std::string path, std::vector<std::string>& files)
{ 
	long   hFile = 0;//File handle
	struct _finddata_t fileInfo;//information of file 
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileInfo)) != -1)
	{
		do
		{
			// If the directory, iteration
			// If not, add to the list
			if ((fileInfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileInfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileInfo.name));
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