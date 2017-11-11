#pragma once
#include "Coordinate.h"
#include <vector>
#include <string>

class Plan
{
public:
	Plan();
	Plan(std::string);
	~Plan();

	friend std::ostream &operator<<(std::ostream &os, const Plan &p);

private :
	//TODO : discuter de la meilleur classe pour stocker ces infos
	unsigned long rows;
	unsigned long columns;
	unsigned long routerRange;
	unsigned long backboneCost; // cost to link one cell to backbone
	unsigned long routerCost; // cost to place one wireless router
	unsigned long maxBudget;


	char **building;
	/*
	# = a wall
	. = a target cell
	_ = a void cell
	*/

	std::vector<Coordinate> routers;
	std::vector<Coordinate> backbones;
};

std::vector<std::string> splitString(std::string str);
std::ostream &operator<<(std::ostream &os, const Plan &p);

