#pragma once
#include "Line.h"
#include "Point.h"
#include "Plane.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

class Figure{ // Окружность
public:
	int id = 0;
//-------------------------------------------------------------------------------------------------------
friend std::ostream& operator<<(std::ostream& os, const Figure& fig) {
		os << "( x - " << fig.O->x << ")^2 + "
		   << "( y - " << fig.O->y << ")^2 + "
		   << "( z - " << fig.O->z << ")^2 = "
		   << fig.r << "^2\n\tPlane: " << *fig.p;
		return os;
	}
//--------------------------------------------------------------------------------------------------------
	// Конструктор: окружность в плоскости Z=0 с центром pt и радиусом range
	Figure(const Point& pt, double range) {
		O = new Point(pt);                 // создаём копию точки
		Point my_point_1 = Point(0, 0, 1) + pt;
		Point my_point_2 = Point(0, 1, 0) + pt;
		p = new Plane(pt, my_point_1, my_point_2);
		r = range;
	}

	// Конструктор: то же, но принимает указатель
	Figure(const Point* pt, double range) {
		O = new Point(*pt);               // создаём копию
		Point my_point_1 = Point(0, 0, 1) + *pt;
		Point my_point_2 = Point(0, 1, 0) + *pt;
		p = new Plane(*pt, my_point_1, my_point_2);
		r = range;
	}
//--------------------------------------------------------------------------------------------------------
	// Конструктор по умолчанию: окружность в плоскости Z=0, центр (0,0,0), радиус 10
	Figure() {
		O = new Point();                    // центр в начале координат
		Point my_point_1 = Point(0, 0, 1);
		Point my_point_2 = Point(0, 1, 0);
		p = new Plane(*O, my_point_1, my_point_2);
		r = 10;
	}
//--------------------------------------------------------------------------------------------------------
	// Конструктор: окружность в заданной плоскости new_plane с центром pt и радиусом new_range
	Figure(const Plane& new_plane, double new_range, const Point& pt) {
		O = new Point(pt);                  // копируем точку
		p = new Plane(new_plane);           // копируем плоскость
		r = new_range;
	}
//--------------------------------------------------------------------------------------------------------------------------
	// Конструктор копирования со смещением по Z на dif
	Figure(const Figure& f, double dif) {
		O = new Point(f.O->x, f.O->y, f.O->z + dif);
		p = new Plane(*f.p, dif);           // смещаем плоскость
		r = f.r;
	}

	// Конструктор копирования со смещением по Z на dif (принимает указатель)
	Figure(const Figure* f, double dif) {
		O = new Point(f->O->x, f->O->y, f->O->z + dif);
		p = new Plane(*f->p, dif);
		r = f->r;
	}
	// Конструктор копирования со смещением по Z на dif (принимает shared_ptr)
	Figure(const std::shared_ptr<Figure>& f, double dif) {
		O = new Point(f->O->x, f->O->y, f->O->z + dif);
		p = new Plane(*f->p, dif);
		r = f->r;
	}
//--------------------------------------------------------------------------------------------------------------------------
	// Проверка: лежит ли точка pt внутри окружности (в плоскости и в пределах радиуса)
	bool In_Figure(const Point& pt) {
		return (*O - pt).len() <= r && p->In_plane(pt);
	}
//--------------------------------------------------------------------------------------------------------------------------
	// Расстояние от точки pt до окружности
	double Dist(const Point& pt) {
		if (p->In_plane(pt)) {
			return std::max(0.0, (*O - pt).len() - r);
		} else {
			return p->Distance(pt);
		}
	}
//--------------------------------------------------------------------------------------------------------------------------
	// Оператор присваивания: копируем данные
	Figure& operator=(const Figure& f) {
		if (this != &f) {                   // защита от самоприсваивания
			delete O;                       // освобождаем старую память
			delete p;
			O = new Point(*f.O);            // копируем точку
			p = new Plane(*f.p);            // копируем плоскость
			r = f.r;
		}
		return *this;
	}
//--------------------------------------------------------------------------------------------------------------------------
	// Деструктор: освобождаем динамическую память
	~Figure() {
		delete O;
		delete p;
	}
//--------------------------------------------------------------------------------------------------------------------------
	Point* O;   // центр окружности
	Plane* p;   // плоскость, в которой лежит окружность
	double r;   // радиус
private:
};
