#pragma once

#include <vector>
#include <list>
#include "Coordinate.h"
const int nbGridCell_side = 10;
class Grid {

public:
	Grid();
	Grid(const int &nbRows, const int &nbColumns);

	Grid(Grid &g);
	Grid(Grid &&g);
	Grid& operator=(Grid &g);
	Grid& operator=(Grid &&g);
	~Grid();

	std::vector<Coordinate>& operator()(std::pair<int, int>);
	bool isWired(std::pair<int, int> pair);
	void setWired(std::pair<int, int> pair, bool setWired);
	std::list<std::pair<int, int>> getNeighbors(std::pair<int, int>);

	inline int getGridCell_width() { return this->gridCell_width; };
	inline int getGridCell_heigth() { return this->gridCell_heigth; };

private:

	std::vector<Coordinate> tab[nbGridCell_side][nbGridCell_side]{};
	bool wired[nbGridCell_side][nbGridCell_side]{};

	int gridCell_width;
	int gridCell_heigth;


};