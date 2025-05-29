#pragma once
#include "Point.h"
#include "Figure.h"

class BuferZone{
private:
    Figure base;
    Plane down;
    double length;
    Point direction;
public:
    BuferZone() {
        base = Figure();
        length = 0;
    }
    BuferZone(Figure f, double len) {
        base = f;
        length = len;
    }
    bool In_Figure(Point p) {
        double dist = down.Distance(p);
        if (dist < 0) {
            return false;
        }
        else {
            return true;
        }
    }
	bool In_Figure(Point& p){
        return true;
	}
    bool In_Figure(Point* p) {
        return true;
    }
};