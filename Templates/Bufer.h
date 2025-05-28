#pragma once
#include "Point.h"

class BuferZone{
private:
    Point p;
    double up;
    double down;
    double range;
public:
    BuferZone() {
        p = Point();
        up = 100;
        down = 0;
        range = 100;
    }
	bool In_Figure(){

	}
};