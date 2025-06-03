#pragma once
#include "Point.h"

class Plane
{
public:
	Plane() {
		v1 = Point();
		v2 = Point();
		p = Point();
		a = v1.y * v2.z - v2.y * v1.z;
		b = -(v1.x * v2.z - v2.x * v1.z);
		c = v1.x * v2.y - v2.x * v1.y;
		d = -p.x * a - p.y * b - p.z * c;
	}
	Plane(Plane &pl, double dif) {
		*this = pl;
		d += dif;
	}
	Plane(Point& A, Point& B, Point& C) {
		v1 = B - A;
		v2 = C - A;
		p = A;
		a = v1.y * v2.z - v2.y * v1.z;
		b = -(v1.x * v2.z - v2.x * v1.z);
		c = v1.x * v2.y - v2.x * v1.y;
		d = -p.x * a - p.y * b - p.z * c;
		n = Point(a, b, c);
		d /= n.len();
		n.norm();
		a = n.x;
		b = n.y;
		c = n.z;
	}
	Plane(Point* A, Point* B, Point* C) {
		v1 = *B - *A;
		v2 = *C - *A;
		p = *A;
		a = v1.y * v2.z - v2.y * v1.z;
		b = -(v1.x * v2.z - v2.x * v1.z);
		c = v1.x * v2.y - v2.x * v1.y;
		d = -p.x * a - p.y * b - p.z * c;
		n = Point(a, b, c);
		d /= n.len();
		n.norm();
		a = n.x;
		b = n.y;
		c = n.z;
	}
	double Distance(Point& p) {
		return a * p.x + b * p.y + c * p.z;
	}
	bool In_plane(Point& p) {
		return abs(a * p.x + b * p.y + c * p.z + d) <= eps;
	}
	Plane operator = (const Plane& p) {
		return p;
	}
private:
	Point v1;
	Point v2;
	Point n;
	Point p;
	double a, b, c, d;
	const double eps = 1e-6;
};
