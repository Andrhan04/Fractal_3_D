#pragma once
#include "..\Templates\Point.h"
#include "..\Templates\Figure.h"

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
        } else {
            if (up->In_Figure(p) || down->In_Figure(p)) {
                return true;
            }
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

    // Указатели заменены на shared_ptr для автоматического управления памятью
    shared_ptr<Figure> up;   // Верхняя фигура
    shared_ptr<Figure> down; // Нижняя фигура

    // Указатели на соседние фракталы в списке (оставлены как сырые указатели,
    // так как они используются для связи объектов, а не владения)
    Fractal* prev = nullptr;
    Fractal* next = nullptr;

private:
    // Закрытая часть класса пока пуста
};
