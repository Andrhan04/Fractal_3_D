#pragma once
#include "..\Templates\Point.h"
#include "..\Templates\Figure.h"
class Fractal
{
public:
	Fractal(Figure &new_up, Figure &new_down) {
		up = new_up;
		down = new_down;
	}
	Fractal(Figure &new_up, Figure &new_down, Fractal &new_left) {
		up = new_up;
		down = new_down;
		left = new_left;
	}
	bool In_Figure(Point& p) {
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
	Fractal *left = nullptr;
	Fractal *right = nullptr;
private:
	Figure up;
	Figure down;
};

