#pragma once
#include "Point.h"
#include "Figure.h"

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
		Point axis = *up->O - *down->O;
		double axisLenSq = axis.dot(axis);
		if (axisLenSq > 0.0) {
			Point fromDown = p - *down->O;
			double t = fromDown.dot(axis) / axisLenSq;
			if (t >= 0.0 && t <= 1.0) {
				Point closest = *down->O + axis * t;
				double dist = (p - closest).len();
				double radius = std::min(up->r, down->r);
				if (dist <= radius) {
					return true;
				}
			}
		}
		else {
			if (up->In_Figure(p) || down->In_Figure(p)) {
				return true;
			}
		}
		return false;
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
		double x1 = down->O->x;
		double x2 = up->O->x;
		double x_min = std::min(x1, x2);
		double x_max = std::max(x1, x2);
		if (x_max - x_min <= 0.0) {
			throw runtime_error("Invalid buffer length along OX");
		}
		double radius = std::min(up->r, down->r);
		if (radius <= 0.0) {
			throw runtime_error("Invalid buffer radius");
		}
		double rx = x_min + (rand() / (double)RAND_MAX) * (x_max - x_min);
		double u = rand() / (double)RAND_MAX;
		double v = rand() / (double)RAND_MAX;
		double rr = std::sqrt(u) * radius;
		double phi = 2.0 * 3.14159265358979323846 * v;
		double cy = down->O->y;
		double cz = down->O->z;
		double ry = cy + rr * std::cos(phi);
		double rz = cz + rr * std::sin(phi);
		return Point(rx, ry, rz);
	}

private:
	// Используем shared_ptr для автоматического управления памятью и избегания утечек
	shared_ptr<Figure> up;
	shared_ptr<Figure> down;
};

