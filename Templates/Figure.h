#pragma once
#include "Line.h"
#include "Point.h"
#include "Plane.h"
#include <iostream>
#include <string>

using namespace std;

class Figure{ // Окружность
public:
	int id = 0;
//-------------------------------------------------------------------------------------------------------
	std::string get_param() { // Информация о окружности
		return "( x - " + std::to_string(O->x) + ")^2 + " + "( y - " + std::to_string(O->y) + ")^2 + " + "( z - " + std::to_string(O->z) + ")^2 = " + std::to_string(r) + "^2\n\tPlane: " + p->get_info();
	}
//--------------------------------------------------------------------------------------------------------
	Figure(Point pt, double range) { // Построение окружности || оси OZ с центром O и радиусом range
		O = &pt;
		Point my_point_1 = Point(1, 0, 0) + pt;
		Point my_point_2 = Point(0, 1, 0) + pt;
		p = new Plane(pt, my_point_1, my_point_2);
		r = range;
	}
//--------------------------------------------------------------------------------------------------------
	Figure() { // Заглушка на построение окружности 
		// On default || OZ and Range = 10 with O = (0, 0, 0)
		O = new Point();
		//cerr << "CREATE O\n";
		Point my_point_1 = Point(1, 0, 0);
		Point my_point_2 = Point(0, 1, 0);
		p = new Plane(*O, my_point_1, my_point_2);
		//cerr << "CREATE Plane\n";
		r = 10;
		//cerr << "CREATE Figure\n";
	}
//--------------------------------------------------------------------------------------------------------
	Figure(Plane &new_plane, double &new_range, Point& pt) {
		O = &pt;
		p = &new_plane;
		r = new_range;
	}
//--------------------------------------------------------------------------------------------------------------------------
	Figure(Figure& f, double dif) {// Построение окружности || другой окружности 
		std::cout << "------------------\n";
		O = new Point(f.O->x, f.O->y, f.O->z + dif);


		//std::cout << Point(f->O->x, f->O->y, f->O->z + dif)->Info();
		std::cout << O->Info();
		p = new Plane(f.p, dif);
		r = f.r;
	}
	Figure(Figure* f, double dif) {// Построение окружности || другой окружности 
		std::cout << "------------------\n";
		O = new Point(f->O->x, f->O->y, f->O->z + dif);

		//std::cout << Point(f.O.x, f.O.y, f.O.z + dif).Info();
		std::cout << O->Info();
		p = new Plane(f->p, dif);
		r = f->r;
	}
//--------------------------------------------------------------------------------------------------------------------------
	bool In_Figure(Point& pt) {
		return (*O - pt).len() <= r && p->In_plane(pt);
	}
//--------------------------------------------------------------------------------------------------------------------------
	double Dist(Point& pt) {
		if (p->In_plane(pt)) {
			return std::max(0.0, (*O - pt).len() - r);
		}
		else {
			return p->Distance(pt);
		}
	}
	Figure operator = (const Figure& f) {
		O = f.O;
		p = f.p;
		r = f.r;
		return f;
	}
	Point *O;
	Plane *p;
	double r;
private:

};