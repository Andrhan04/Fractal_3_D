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
		//cerr << "DO BuferZone IN 2 PLANE\n";
		cout << "done BuferZone IN 2 PLANE\n";
	}

	// Оператор вывода в поток: выводит верхнюю и нижнюю фигуры
	friend std::ostream& operator<<(std::ostream& os, const BuferZone& bz) {
		try {
			os << "up = " << *bz.up << "\ndown = " << *bz.down;
			return os;
		}
		catch (...) {
			throw runtime_error("Error in operator << for BuferZone");
		}
	}

	// Проверка, находится ли точка внутри буферной зоны
	bool In_Figure(Point& p) {
		double dist_out_dn = down->Dist(p);
		double dist_out_up = up->Dist(p);
		//cerr << dist_out_up << ' ' << dist_out_dn << endl;
		if (dist_out_dn * dist_out_up <= 0) {
			// Создаём вспомогательную фигуру на основе верхней и расстояния до точки
			auto help = make_shared<Figure>(up, dist_out_up);
			return help->In_Figure(p);
		}
		else {
			return false;
		}
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
		int dist = down->Dist(*up->O);
		if (dist < 1) {
			throw runtime_error("Too small distance between planes");
		}
		auto help = make_shared<Figure>(down, dist);
		double x = help->O->x + (rand() / (double)RAND_MAX - 0.5) * 2 * help->r;
		double y = help->O->y + (rand() / (double)RAND_MAX - 0.5) * 2 * help->r;
		double z = help->O->z + (rand() / (double)RAND_MAX - 0.5) * 2 * help->r;
		return Point(x, y, z);
	}

private:
	// Используем shared_ptr для автоматического управления памятью и избегания утечек
	shared_ptr<Figure> up;
	shared_ptr<Figure> down;
};

