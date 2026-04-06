#pragma once
#include "..\Templates\Point.h"
#include "..\Templates\Trap.h"
#include "..\Templates\Figure.h"
#include "..\Templates\Generator.h"
#include <math.h>
#include <cstdlib>

using namespace std;

class Fractal
{
public:
    // Конструктор по умолчанию: создаёт два объекта Figure, связанных между собой
    Fractal() : up(make_shared<Figure>()), down(make_shared<Figure>(up, 100.0)) {
        // cerr << "DEFAULT FRACTAL" << endl;
    }

    // Конструктор с двумя фигурами: принимает shared_ptr вместо сырых указателей
    Fractal(shared_ptr<Figure> new_up, shared_ptr<Figure> new_down)
        : up(new_up), down(new_down) {
        // cerr << "DO FRACTAL IN 2 PLANE\n";
    }

    // Оператор вывода в поток: выводит обе фигуры
    friend std::ostream& operator<<(std::ostream& os, const Fractal& fr) {
		if (fr.next != nullptr) os << *fr.next << endl;
        os << "up = " << *fr.up << "\ndown = " << *fr.down;
        return os;
    }

    // Проверка, содержится ли точка в фигуре фрактала
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
                if (next != nullptr) return next->In_Figure(p);
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
                if (next != nullptr) return next->In_Figure(p);
                return false;
            }
        }
        Point center(a.x + (b.x - a.x) * t,
                     a.y + (b.y - a.y) * t,
                     a.z + (b.z - a.z) * t);
        double dy = p.y - center.y;
        double dz = p.z - center.z;
        if (dy * dy + dz * dz <= radius * radius + EPS) {
            return true;
        }
        if (next != nullptr) {
            return next->In_Figure(p);
        }
        return false;
    }

    // Оператор присваивания: возвращает копию фрактала
    Fractal operator=(const Fractal& f) {
        if (this != &f) {
            up = f.up;
            down = f.down;
        }
        return *this;
    }

    // Генерация случайной точки на боковой поверхности цилиндра
	// Цилиндры наклонные, но основания перпендикулярны оси OX
	Point generate_point() {
		// Генерируем случайный угол от 0 до 2π
		double angle = gen.get_double(0.0, 2.0 * PI);
		
		// Генерируем случайную позицию вдоль оси цилиндра между центрами оснований
		double t = gen.get_double(0.0, 1.0);
		
		// Вычисляем центр на оси цилиндра в позиции t
		Point center(
			down->O->x + t * (up->O->x - down->O->x),
			down->O->y + t * (up->O->y - down->O->y),
			down->O->z + t * (up->O->z - down->O->z)
		);
		
		// Используем средний радиус двух окружностей
		double avg_radius = (down->r + up->r) / 2.0;
		
		// Поскольку основания перпендикулярны OX, нормаль к основаниям - это ось OX
		// Создаем два ортогональных вектора в плоскости, перпендикулярной OX
		// Эти векторы будут лежать в плоскости оснований (YZ-плоскость, но смещенная)
		Point perp1(0.0, 1.0, 0.0);  // Вектор вдоль оси Y
		Point perp2(0.0, 0.0, 1.0);  // Вектор вдоль оси Z
		
		// Вычисляем точку на поверхности цилиндра
		// Смещаем от центра в плоскости, перпендикулярной OX
		Point surface_point = center + perp1 * (avg_radius * cos(angle)) + perp2 * (avg_radius * sin(angle));
		
		return surface_point;
	}

    // Указатели заменены на shared_ptr для автоматического управления памятью
    shared_ptr<Figure> up;   // Верхняя фигура
    shared_ptr<Figure> down; // Нижняя фигура

    // Указатели на соседние фракталы в списке (оставлены как сырые указатели,
    // так как они используются для связи объектов, а не владения)
    Fractal* prev = nullptr;
    Fractal* next = nullptr;

    void addTrap(Trap* trap) {
        for (size_t i = 0; i < traps.size(); i++) {
            if (traps[i]->distance(trap->position) <= 1) {
                return;
            }
        }
        traps.push_back(trap);
	}

    bool check_cath(Point& p) {
        for (Trap* trap : traps) {
            if (trap->is_catch(&p)) {
                return true;
            }
        }
        return false;
	}


    int get_traps_count() const {
        return traps.size();
	}

    void out_traps() const {
        for (auto i : traps) {
            cout << *i << endl;
        }
    }

private:
    // Закрытая часть класса пока пуста
	vector<Trap*> traps; // Вектор указателей на ловушки, связанных с этим фракталом
    Generator gen = Generator();
    double PI = acos(-1);
};
