#include "Coordinate.h"


Coordinate::Coordinate(unsigned long _x, unsigned long _y) : x(_x), y(_y)
{
}


Coordinate::~Coordinate()
{
}

std::ostream &operator<<(std::ostream &os, const Coordinate &c) {
	os << "(" << c.x << ", " << c.y << ")" << std::endl;
	return os;
}
