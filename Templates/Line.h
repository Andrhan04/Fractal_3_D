#pragma once

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

};