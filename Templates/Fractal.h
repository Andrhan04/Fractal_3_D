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
        // Проверка попадания точки в цилиндр:
        // Точка внутри цилиндра, если её проекция лежит между up и down
        // и расстояние до оси цилиндра меньше радиуса.
        double distAxis = up->DistAxis(p);   // расстояние до оси цилиндра
        double hUp = up->Height(p);          // высота по отношению к up
        double hDown = down->Height(p);      // высота по отношению к down

        // Проверяем, что точка между плоскостями up и down, и внутри радиуса
        if (hUp * hDown <= 0 && distAxis <= up->r) {
            return true;
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
