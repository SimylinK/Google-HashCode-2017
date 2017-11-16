#pragma once

#include <ostream>

class Coordinate
{
public:
	Coordinate();
	Coordinate(int x, int y);
	~Coordinate();
	friend std::ostream &operator<<(std::ostream &, const Coordinate &);
	int x;
	int y;
};

std::ostream &operator<<(std::ostream &os, const Coordinate &c);
