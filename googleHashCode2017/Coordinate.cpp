#include "Coordinate.h"

Coordinate::Coordinate(): x(0), y(0) {
}

Coordinate::Coordinate(int _x, int _y) : x(_x), y(_y)
{
}

Coordinate::~Coordinate()
{
}

std::ostream &operator<<(std::ostream &os, const Coordinate &c) {
	os << "(" << c.x << ", " << c.y << ")" << std::endl;
	return os;
}


