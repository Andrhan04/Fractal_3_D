#pragma once
#include "Point.h"
#include "Figure.h"

class BuferZone{
private:
    Figure up;
    Figure down;
    double length;
    Point direction;
public:
    BuferZone() {
        up = Figure();
        down = Figure();
        length = 0;
        direction = Point(0, 0, 0);
    }
    BuferZone(Figure &new_up, Figure& new_down) {
        up = Figure();
        down = Figure();
    }
    BuferZone(Figure u, Figure d, double len) {
        down = d;
        up = u;
        length = len;
        direction = u.O - d.O;
    }
	bool In_Figure(Point& p){
        double dist_out_dn = down.Dist(p);
        double dist_out_up = up.Dist(p);
        if (dist_out_dn * dist_out_up < 0) {
            Figure help = Figure(up, dist_out_up);
            return help.In_Figure(p);
        }
        else {
            return false;
        }
	}
};