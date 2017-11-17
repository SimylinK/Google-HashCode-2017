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

bool operator==(Coordinate a, Coordinate b) {
	if (a.x == b.x && a.y == b.y) {
		return true;
	}
	else return false;
}

bool operator!=(Coordinate a, Coordinate b) {
	return !(a == b);
}

/**
* Compute the octogonal distance between 2 points
* @param a: a point
* @param b: a point
* @return the distance between a and b points
*/
const int& distance(const Coordinate &a, const Coordinate &b)
{
	int deltaX = std::abs(a.x - b.x);
	int deltaY = std::abs(a.y - b.y);
	return std::max(deltaX, deltaY);
}