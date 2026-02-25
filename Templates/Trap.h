#pragma once
#include <cmath>
#include "../Templates/Point.h"

class Trap {
public:
    /** @brief Позиция ловушки */
    Point *position;
    /** @brief Радиус ловушки */
    double radius;

	bool alive = true; // Флаг, указывающий, активна ли ловушка

    /** @brief Базовый конструктор ловушки */
    Trap() : position(new Point()), radius(1.0) {}

    /** @brief Конструктор ловушки с параметрами */
    Trap(Point *position, double radius) {
        this->position = position;
        this->radius = radius;
    }

    /** @brief Деструктор ловушки */
    ~Trap() {
        delete position;
    }

    /** @brief Проверка, поймана ли частица */
    bool is_catch(Point *point) {
        if ((*point - *position).len() <= radius) {
            alive = false;
            return true;
        }
        else {
			return false;
        }
    }
    
    /** @brief Возвращает расстояние до частицы */
    double distance(Point *point) {
        return (*point - *position).len();
    }

    /** @brief Оператор вывода в поток */
    friend std::ostream& operator<<(std::ostream& os, const Trap& trap) {
        os << "Trap(position: " << *trap.position << ", radius: " << trap.radius << ")";
        return os;
    }
};