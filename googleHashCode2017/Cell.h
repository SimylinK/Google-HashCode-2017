#ifndef GOOGLEHASHCODE2017_CELL_H
#define GOOGLEHASHCODE2017_CELL_H


#include "Coordinate.h"

class Cell {
public:
	Cell();
	Cell(Coordinate &coord, const char &c);
	Cell(const Cell&);
	Cell(Cell&&);
	Cell& operator=(const Cell&);
	Cell& operator=(Cell&&);
	~Cell();
	// setters and getters
	inline bool isWired(){return wired;}
	inline bool hasRouter(){return router;}
	inline bool isCovered(){return covered;}
	inline void setWired(bool _wired){wired=_wired;}
	inline void setRouter(bool _router){router=_router;}
	inline void setCovered(bool _covered){covered=_covered;}
	inline Coordinate getCoordinate(){return coordinate;}
	inline void setCoordinate(const Coordinate &c){coordinate = c;}
	inline char floorType(){return environment;}
	inline void setEnvironment(const char &c){environment = c;}
private:
	Coordinate coordinate;
	bool wired = false;
	bool router = false;
	bool covered = false; // true if the case floorType WiFi-covered
	char environment;

};


#endif //GOOGLEHASHCODE2017_CELL_H
