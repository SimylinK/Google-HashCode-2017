#pragma once
#include "Coordinate.h"
#include <vector>

template <unsigned int H, unsigned int W>
class Plan
{
public:
	Plan();
	~Plan();

private :
	//TODO : discuter de la meilleur classe pour stocker ces infos
	
	int building[H][W];
	/*
	# = a wall
	. = a target cell
	_ = a void cell
	*/

	std::vector<Coordinate> routers;
	std::vector<Coordinate> backbones;
};

#include "Plan.hxx"
