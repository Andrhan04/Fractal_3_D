#pragma once
#include "Point.h"
#include "Figure.h"

class BuferZone{
private:
    Figure *up;
    Figure *down;
public:
    BuferZone() { // Заглушка
        up = new Figure();
        down = new Figure();
    }
    string get_info() {
		return "up = " + up->get_param() + "\ndown = " + down->get_param();
	}
    BuferZone(Figure &new_up, Figure& new_down) {
        up = &new_up;
        down = &new_down;
    }
    BuferZone(Figure u, Figure d) {
        down = &d;
        up = &u;
    }
    BuferZone(Figure *u, Figure *d) {
        down = d;
        up = u;
    }
	bool In_Figure(Point& p){
        double dist_out_dn = down->Dist(p);
        double dist_out_up = up->Dist(p);
        if (dist_out_dn * dist_out_up < 0) {
            Figure help = Figure(up, dist_out_up);
            return help.In_Figure(p);
        }
        else {
            return false;
        }
	}
};