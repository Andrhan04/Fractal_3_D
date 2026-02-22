#pragma once
#include <cmath>          // Более безопасный C++-вариант math.h
#include <string>
#include <iostream>

// Убираем using namespace std; в заголовке, чтобы избежать загрязнения глобального пространства имён

/**
 * @brief Класс, описывающий точку/вектор в трёхмерном пространстве.
 *        Все операции, где возможно, теперь принимают и возвращают ссылки/указатели
 *        вместо копирования объектов.
 */
class Point
{
public:
    /* ------------------------------------------------------------------ */
    /*                         Конструкторы                               */
    /* ------------------------------------------------------------------ */
    /** @brief Конструктор с координатами */
    Point(double X, double Y, double Z) : x(X), y(Y), z(Z) {}

    /** @brief Конструктор по умолчанию (нулевая точка) */
    Point() : x(0.0), y(0.0), z(0.0) {}

    /* ------------------------------------------------------------------ */
    /*                        Базовые методы                              */
    /* ------------------------------------------------------------------ */
    /** @brief Возвращает строку-описание точки */
    std::string Info() const
    {
        return "Point: " + std::to_string(x) + " " +
               std::to_string(y) + " " + std::to_string(z) + "\n";
    }

    /** @brief Перегрузка вывода в поток */
    friend std::ostream& operator<<(std::ostream& os, const Point& p)
    {
        os << "Point: " << p.x << " " << p.y << " " << p.z << "\n";
        return os;
    }

    /* ------------------------------------------------------------------ */
    /*                        Геометрические операции                     */
    /* ------------------------------------------------------------------ */
    /** @brief Нормализует вектор (делает единичным) */
    void norm()
    {
        double length = len();
        if (length != 0.0)
            *this = *this / length;
    }

    /** @brief Длина (модуль) вектора */
    double len() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    /** @brief Скалярное произведение */
    double dot(const Point& p) const
    {
        return x * p.x + y * p.y + z * p.z;
    }

    /** @brief Векторное произведение */
    Point cross(const Point& p) const
    {
        return Point(y * p.z - z * p.y,
                     z * p.x - x * p.z,   // убираем лишний минус
                     x * p.y - y * p.x);
    }

    /* ------------------------------------------------------------------ */
    /*                     Арифметические операторы                       */
    /* ------------------------------------------------------------------ */
    /* Операции с объектами Point (по ссылке) */
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y, z - p.z); }
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y, z + p.z); }

    /* Операции с указателями Point (вместо копирования – работаем через указатели) */
    Point operator-(const Point* p) const { return Point(x - p->x, y - p->y, z - p->z); }
    Point operator+(const Point* p) const { return Point(x + p->x, y + p->y, z + p->z); }

    /* Умножение и деление на скаляр */
    Point operator*(double scalar) const { return Point(x * scalar, y * scalar, z * scalar); }
    Point operator/(double scalar) const { return Point(x / scalar, y / scalar, z / scalar); }

    /* ------------------------------------------------------------------ */
    /*                     Операторы присваивания и сравнения            */
    /* ------------------------------------------------------------------ */
    Point& operator=(const Point& p)
    {
        if (this != &p)   // защита от самоприсваивания
        {
            x = p.x;
            y = p.y;
            z = p.z;
        }
        return *this;
    }

    /* Сравнение на равенство/неравенство */
    bool operator==(const Point& p) const { return x == p.x && y == p.y && z == p.z; }
    bool operator==(const Point* p) const { return p && x == p->x && y == p->y && z == p->z; }

    bool operator!=(const Point& p) const { return !(*this == p); }
    bool operator!=(const Point* p) const { return !(*this == p); }

    /* ------------------------------------------------------------------ */
    /*                          Указатели                                 */
    /* ------------------------------------------------------------------ */
    Point*       toPointer()       { return this; }
    const Point* toPointer() const { return this; }

    /* ------------------------------------------------------------------ */
    /*                          Данные                                    */
    /* ------------------------------------------------------------------ */
    double x{}, y{}, z{};  // инициализация по умолчанию через {}

private:
    /* Пока нет приватных методов/данных */
};
