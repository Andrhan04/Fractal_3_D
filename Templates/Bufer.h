#pragma once
#include "Point.h"
#include "Figure.h"
#include "Generator.h"
#include <algorithm>
#include <cmath>

using namespace std;

class BuferZone
{
public:
	// Конструктор по умолчанию: создаёт две фигуры (верхнюю и нижнюю) с разницей 100.0
	BuferZone() : up(make_shared<Figure>()), 
	              down(make_shared<Figure>(up, 100.0)) {
		//cerr << "DEFAULT BuferZone" << endl;
	}

	// Конструктор с заданными фигурами: принимает shared_ptr на верхнюю и нижнюю фигуры
	BuferZone(shared_ptr<Figure> new_up, shared_ptr<Figure> new_down) 
		: up(new_up), down(new_down) {
		//cout << "done BuferZone IN 2 PLANE\n";
	}

	// Оператор вывода в поток: выводит верхнюю и нижнюю фигуры
	friend std::ostream& operator<<(std::ostream& os, const BuferZone& bz) {
		try {
			os << "up = " << *bz.up << "\ndown = " << *bz.down;
			return os;
		}
		catch (const exception& e) {
			cout << "ERROR IS \n" << e.what() << endl;
			throw runtime_error("Error in operator << for BuferZone");
		}
	}

	// Проверка, находится ли точка внутри буферной зоны
	bool In_Figure(Point& p) {
		const double EPS = 1e-9;
		double radius = std::min(up->r, down->r);
		Point a = *down->O;
		Point b = *up->O;
		double dx = b.x - a.x;
		double t = 0.0;
		if (std::abs(dx) > EPS) {
			t = (p.x - a.x) / dx;
			if (t < 0.0 || t > 1.0) {
				return false;
			}
		} else {
			Point v(b.y - a.y, b.z - a.z, 0.0);
			double vlen2 = v.y * v.y + v.z * v.z;
			if (vlen2 > EPS) {
				Point w(p.y - a.y, p.z - a.z, 0.0);
				t = (w.y * v.y + w.z * v.z) / vlen2;
				if (t < 0.0) t = 0.0;
				if (t > 1.0) t = 1.0;
			} else {
				t = 0.0;
			}
			if (std::abs(p.x - a.x) > EPS) {
				return false;
			}
		}
		Point center(a.x + (b.x - a.x) * t,
		             a.y + (b.y - a.y) * t,
		             a.z + (b.z - a.z) * t);
		double dy = p.y - center.y;
		double dz = p.z - center.z;
		return (dy * dy + dz * dz <= radius * radius + EPS);
	}

	// Оператор присваивания: копирует shared_ptr (разделяемое владение)
	BuferZone& operator=(const BuferZone& f) {
		if (this != &f) {
			up = f.up;
			down = f.down;
		}
		return *this;
	}

	// Генерация случайной точки внутри буферной зоны
	Point generate_point(){
		if (!up || !down) {
			throw runtime_error("Buffer zones are not initialized");
		}
		Point axis = *up->O - *down->O;
		double length = axis.len();
		if (length <= 0.0) {
			throw runtime_error("Invalid buffer length");
		}
		Point dir = axis / length;
		double radius = std::min(up->r, down->r);
		if (radius <= 0.0) {
			throw runtime_error("Invalid buffer radius");
		}
		Point notAxis;
		if (std::abs(dir.x) < 0.9) {
			notAxis = Point(1.0, 0.0, 0.0);
		}
		else {
			notAxis = Point(0.0, 1.0, 0.0);
		}
		Point perp1 = dir.cross(notAxis);
		perp1.norm();
		Point perp2 = dir.cross(perp1);
		perp2.norm();
		double t = generator_.get_double(0.0, length);
		Point base = *down->O + dir * t;
		double u = generator_.get_double(0.0, 1.0);
		double v = generator_.get_double(0.0, 1.0);
		double rr = std::sqrt(u) * radius;
		double phi = 2.0 * 3.14159265358979323846 * v;
		Point offset = perp1 * (rr * std::cos(phi)) + perp2 * (rr * std::sin(phi));
		Point result = base + offset;
		return result;
	}

private:
	// Используем shared_ptr для автоматического управления памятью и избегания утечек
	shared_ptr<Figure> up;
	shared_ptr<Figure> down;
	Generator generator_;
};

