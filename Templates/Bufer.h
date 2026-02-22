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
		cout << "done BuferZone IN 2 PLANE\n";
	}
	friend std::ostream& operator<<(std::ostream& os, const BuferZone& bz) {
		try {
			os << "up = " << *bz.up << "\ndown = " << *bz.down;
			return os;
		}
		catch (...) {
			throw runtime_error("Error in operator << for BuferZone");
		}
	}
	bool In_Figure(Point& p) {
		
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
		int dist = down->Dist(*up->O);
		if (dist < 1) {
			throw runtime_error("Too small distance between planes");
		}
		Figure* help = new Figure(down, dist);
		double x = help->O->x + (rand() / (double)RAND_MAX - 0.5) * 2 * help->r;
		double y = help->O->y + (rand() / (double)RAND_MAX - 0.5) * 2 * help->r;
		double z = help->O->z + (rand() / (double)RAND_MAX - 0.5) * 2 * help->r;
		return Point(x, y, z);
    }

private:

};

