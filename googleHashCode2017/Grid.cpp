
#include "Grid.h"

Grid::Grid() {}; 
Grid::Grid(Plan &p) {

	for (int i = 0; i < nbGridCell_side; i++) {
		for (int j = 0; j < nbGridCell_side; j++) {
			this->wired[i][j] = false;
		}

	}
	this->gridCell_heigth = p.getRows() / nbGridCell_side;
	this->gridCell_width = p.getColumns() / nbGridCell_side;
}

std::vector<Coordinate>& Grid::operator()(std::pair<int, int> pair) {

	if (pair.first<0 || pair.first>nbGridCell_side ||
		pair.second<0 || pair.second>nbGridCell_side) 
		throw std::out_of_range("Grid::operator() : index out of range");

	return this->tab[pair.first][pair.second];

}

bool Grid::isWired(std::pair<int, int> pair) {

	if (pair.first<0 || pair.first>nbGridCell_side ||
		pair.second<0 || pair.second>nbGridCell_side)
		throw std::out_of_range("Grid::operator() : index out of range");

	return this->wired[pair.first][pair.second];
}

std::list<std::pair<int, int>> Grid::getNeighbors(std::pair<int, int> pair) {

	std::list<std::pair<int,int>> neighborsList;

	int i = pair.first;
	int j = pair.second;

	if (i != nbGridCell_side - 1) neighborsList.push_back(std::pair<int,int>(i + 1,j));
	if (j != nbGridCell_side - 1) neighborsList.push_back(std::pair<int, int>(i,j+1));
	if (i != 0) neighborsList.push_back(std::pair<int, int>(i - 1,j));
	if (j != 0) neighborsList.push_back(std::pair<int, int>(i,j - 1));

	return neighborsList;
}
