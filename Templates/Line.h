#pragma once
#include "Point.h"

class Line {
private:
	Point begin;
	Point end;
	Point vec;
public:
	Line() {
		begin = Point(0, 0, 0);
		end = Point(100, 100, 100);
		vec = end - begin;
	}
	Line(Point b, Point e) {
		begin = b;
		end = e;
		vec = end - begin;
	}
};