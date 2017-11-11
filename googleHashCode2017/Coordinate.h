#pragma once

#include <ostream>

class Coordinate
{
public:
	Coordinate(unsigned long x, unsigned long y);
	~Coordinate();
	friend std::ostream &operator<<(std::ostream &, const Coordinate &);

private:
	unsigned long x;
	unsigned long y;
};

std::ostream &operator<<(std::ostream &os, const Coordinate &c);
