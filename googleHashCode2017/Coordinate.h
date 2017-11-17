#pragma once

#include <ostream>
#include <cstdlib>
#include <algorithm>

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
bool operator==(Coordinate a, Coordinate b);
bool operator!=(Coordinate a, Coordinate b);

const int& distance(const Coordinate &a, const Coordinate &b);