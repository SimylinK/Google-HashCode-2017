
#include "Grid.h"
#include <iostream>
#include <cmath>

Grid::Grid() {};

Grid::Grid(const int &nbRows, const int &nbColumns) {

	for (int i = 0; i < nbGridCell_side; i++) {
		for (int j = 0; j < nbGridCell_side; j++) {
			this->wired[i][j] = false;
		}
	}
	(nbColumns%nbGridCell_side == 0) ? gridCell_width = nbColumns / nbGridCell_side : gridCell_width = (nbColumns / nbGridCell_side) + 1;
	(nbRows%nbGridCell_side == 0) ? gridCell_heigth = nbRows / nbGridCell_side : gridCell_width = (nbRows / nbGridCell_side) + 1;
}

Grid::Grid(Grid &g) {
	for (int i = 0; i < nbGridCell_side; i++) {
		for (int j = 0; j < nbGridCell_side; j++) {
			this->wired[i][j] = g.wired[i][j];		
		}
	}
	this->gridCell_heigth = g.getGridCell_heigth();
	this->gridCell_width = g.getGridCell_width();

}
Grid::Grid(Grid &&g) {
	for (int i = 0; i < nbGridCell_side; i++) {
		for (int j = 0; j < nbGridCell_side; j++) {
			this->wired[i][j] = g.wired[i][j];
		}
	}
	this->gridCell_heigth = g.getGridCell_heigth();
	this->gridCell_width = g.getGridCell_width();

}
Grid& Grid::operator=(Grid &g){
	for (int i = 0; i < nbGridCell_side; i++) {
		for (int j = 0; j < nbGridCell_side; j++) {
			this->wired[i][j] = g.wired[i][j];
		}
	}
	this->gridCell_heigth = g.getGridCell_heigth();
	this->gridCell_width = g.getGridCell_width();

	return *this;

}
Grid& Grid::operator=(Grid &&g){

	for (int i = 0; i < nbGridCell_side; i++) {
		for (int j = 0; j < nbGridCell_side; j++) {
			this->wired[i][j] = g.wired[i][j];
		}
	}
	this->gridCell_heigth = g.getGridCell_heigth();
	this->gridCell_width = g.getGridCell_width();

	return *this;

}
Grid::~Grid(){}



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

void Grid::setWired(std::pair<int, int> pair, bool setWired) {

	if (pair.first<0 || pair.first>nbGridCell_side ||
		pair.second<0 || pair.second>nbGridCell_side)
		throw std::out_of_range("Grid::operator() : index out of range");

	this->wired[pair.first][pair.second] = setWired;
}

std::list<std::pair<int, int>> Grid::getNeighbors(std::pair<int, int> pair) {

	std::list<std::pair<int,int>> neighborsList;

	int i = pair.first;
	int j = pair.second;

	if (i < nbGridCell_side - 1) {neighborsList.push_back(std::pair<int, int>(i + 1, j));}
	if (j < nbGridCell_side - 1) {neighborsList.push_back(std::pair<int, int>(i, j + 1));}
	if (i != 0) {neighborsList.push_back(std::pair<int, int>(i - 1, j));}
	if (j != 0) { neighborsList.push_back(std::pair<int, int>(i, j - 1)); }
	return neighborsList;
}
