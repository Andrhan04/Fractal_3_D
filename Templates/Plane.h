#pragma once
#include "Point.h"
#include <string>

using namespace std;

class Plane
{
public:
	Plane() {
		v1 = new Point();
		v2 = new Point();
		p = new Point();
		a = v1->y * v2->z - v2->y * v1->z;
		b = -(v1->x * v2->z - v2->x * v1->z);
		c = v1->x * v2->y - v2->x * v1->y;
		d = -p->x * a - p->y * b - p->z * c;
	}

	Plane(Plane &pl, double dif) {
		*this = pl;
		(*this).d += dif;
	}

	Plane(Plane* pl, double dif) {
		v1 = pl->v1;
		v2 = pl->v2;
		p = pl->p;
		a = pl->a;
		b = pl->b;
		c = pl->c;
		d = pl->d + dif;
		n = pl->n;
	}

	Plane(Point& A, Point& B, Point& C) {
		v1 = (B - A).toPointer();
		v2 = (C - A).toPointer();
		p = &A;
		a = v1->y * v2->z - v2->y * v1->z;
		b = -(v1->x * v2->z - v2->x * v1->z);
		c = (*v1).x * (*v2).y - (*v2).x * (*v1).y;
		d = -p->x * a - p->y * b - p->z * c;
		n = new Point(a, b, c);
		d /= n->len();
		n->norm();
		a = n->x;
		b = n->y;
		c = n->z;
	}
	Plane(Point* A, Point* B, Point* C) {
		v1 = (*B - *A).toPointer();
		v2 = (*C - *A).toPointer();
		p = A;
		a = v1->y * v2->z - v2->y * v1->z;
		b = -(v1->x * v2->z - v2->x * v1->z);
		c = v1->x * v2->y - v2->x * v1->y;
		d = -p->x * a - p->y * b - p->z * c;
		n = new Point(a, b, c);
		d /= n->len();
		n->norm();
		a = n->x;
		b = n->y;
		c = n->z;
	}
	string get_info() {
		return to_string(a) + "x + (" + to_string(b) + ")y + (" + to_string(c) + ")z + (" + to_string(d) + ") = 0\n";
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
	Point *v1;
	Point *v2;
	Point *n;
	Point *p;
	double a, b, c, d;
private:
	const double eps = 1e-6;
};
