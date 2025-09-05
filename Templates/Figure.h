#pragma once
#include "Line.h"
#include "Point.h"
#include "Plane.h"
#include <string>

class Figure{ // Окружность
public:
	int id = 0;
	std::string get_param() {
		return "( x - " + std::to_string(O.x) + ")^2" + "( y - " + std::to_string(O.y) + ")^2" + "( z - " + std::to_string(O.z) + ")^2 = " + std::to_string(r) + "^2";
	}
	Figure(Point pt, double range) {
		O = pt;
		p = Plane(pt, pt, pt);
		r = range;
	}
	Figure() {
		p = Plane();
		O = Point();
		r = 0;
	}
	Figure(Plane &new_plane, double &new_range) {
		p = new_plane;
		r = new_range;
	}
	Figure(Figure& f,double dif) {
		*this = f;
		p = Plane(p, dif);
	}
	bool In_Figure(Point& pt) {
		return (O - pt).len() <= r && p.In_plane(pt);
	}
	double Dist(Point& pt) {
		if (p.In_plane(pt)) {
			return std::max(0.0, (O - pt).len() - r);
		}
		else {
			return p.Distance(pt);
		}
	}
	Point O;
private:
	Plane p;
	double r;
};