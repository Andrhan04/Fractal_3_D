#pragma once
#include "Point.h"
#include "Figure.h"

using namespace std;

class BuferZone
{
public:
	BuferZone() {
		//cerr << "DEFAULT BuferZone" << endl;
		up = new Figure();
		double dif = 100.0;
		down = new Figure(up, dif);
	}
	BuferZone(Figure* new_up, Figure* new_down) {
		//cerr << "DO BuferZone IN 2 PLANE\n";
		up = new_up;
		down = new_down;
	}
	string get_info() {
		return "up = " + up->get_param() + "\ndown = " + down->get_param();
	}
	bool In_Figure(Point& p) {
		//cerr << "Check IS point in Segment in BuferZone\n";
		double dist_out_dn = down->Dist(p);
		double dist_out_up = up->Dist(p);
		//cerr << dist_out_up << ' ' << dist_out_dn << endl;
		if (dist_out_dn * dist_out_up <= 0) {
			Figure *help = new Figure(up, dist_out_up);
			return help->In_Figure(p);
		}
		else {
			return false;
		}
	}
	BuferZone operator = (const BuferZone& f) {
		return f;
	}

	Figure *up;
	Figure *down;
	
    Point generate_point(){
        return Point();
    }

private:

};

