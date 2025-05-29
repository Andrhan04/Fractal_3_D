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
	bool In_Figure(Point& p) {
		return (O - p).len() <= r;
	}
private:
	Point O;
	Plane p;
	double r;
};