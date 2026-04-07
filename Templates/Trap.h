#pragma once
#include <cmath>
#include <iostream>
#include "../Templates/Point.h"

class Trap {
public:
    /** @brief Позиция ловушки */
    Point *position;
    int segment_id = 0;
    /** @brief Радиус ловушки */
    double radius = 0.5;

	bool alive = true; // Флаг, указывающий, активна ли ловушка

    /** @brief Базовый конструктор ловушки */
    Trap() : position(new Point()), radius(0.5), segment_id(0) {}

    /** @brief Конструктор ловушки с параметрами */
    Trap(int segment, Point* position, double radius) {
        this->position = position;
        this->radius = radius;
        this->segment_id = segment;
    }

    /** @brief Конструктор ловушки с параметрами */
    Trap(Point *position, int segment) {
        this->position = position;
        this->radius = 0.5;
        this->segment_id = segment;
    }

    /** @brief Перегрузка вывода в поток */
    friend std::ostream& operator<<(std::ostream& os, const Trap& t){
        if (t.alive) {
            os << t.segment_id << " " << *t.position << " " << t.radius;
        }
        else {
            os << "DEAD " << *t.position;
        }
        return os;
    }

    bool operator<(const Trap& other) const { return (segment_id < other.segment_id) || (segment_id == other.segment_id && position < other.position); }
    bool operator<(const Trap* other) const { return (segment_id < other->segment_id) || (segment_id == other->segment_id && position < other->position); }
    bool operator>(const Trap& other) const { return (segment_id > other.segment_id) || (segment_id == other.segment_id && position > other.position); }
    bool operator>(const Trap* other) const { return (segment_id > other->segment_id) || (segment_id == other->segment_id && position > other->position); }

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


};