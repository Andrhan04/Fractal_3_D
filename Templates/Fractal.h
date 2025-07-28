#pragma once
#include "..\Templates\Point.h"

class Fractal
{
public:
	Fractal();
	bool In_Figure(Point& p) {
		// 
		return true;
	}
	Fractal *left;
	Fractal *right;
private:

};

