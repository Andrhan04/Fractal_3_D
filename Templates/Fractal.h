#pragma once
#include "..\Templates\Point.h"
#include "..\Templates\Figure.h"

using namespace std;

class Fractal
{
public:
	Fractal() {
		cerr << "DEFAULT FRACTAL" << endl;
		up = new Figure();
		double dif = 100.0;
		down = new Figure(up, dif);
	}
	Fractal(Figure* new_up, Figure* new_down) {
		cerr << "DO FRACTAL IN 2 PLANE\n";
		up = new_up;
		down = new_down;
	}
	string get_info() {
		return "up = " + up->get_param() + "\ndown = " + down->get_param();
	}
	Fractal(Figure *new_up, Figure *new_down, Fractal *new_left, Fractal* new_right) {
		up = new_up;
		down = new_down;
		left = new_left;
		right = new_right;
	}
	bool In_Figure(Point& p) {
		double dist_out_dn = down->Dist(p);
		double dist_out_up = up->Dist(p);
		cerr << dist_out_up << ' ' << dist_out_dn << endl;
		if (dist_out_dn * dist_out_up < 0) {
			Figure *help = new Figure(up, dist_out_up);
			return help->In_Figure(p);
		}
		else {
			return false;
		}
	}
	Fractal operator = (const Fractal& f) {
		return f;
	}
	Fractal *left = nullptr;
	Fractal *right = nullptr;
	Figure *up;
	Figure *down;
private:

};

