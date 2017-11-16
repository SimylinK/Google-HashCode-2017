#include <cmath>
#include "Solver.h"

void placeRoutersIterative(Plan &p) {
	int maxReachableCells = (p.getRouterRange()*2 + 1)*(p.getRouterRange()*2 + 1);

	for(int i=0; i<p.getRows(); i++){
		for (int j=0; j<p.getColumns(); j++) {
			if (p.coverableCells(Coordinate(i, j)).size() == maxReachableCells) {
				Coordinate c(i,j);
				p.addRouter(c);
			}
		}
	}
}
