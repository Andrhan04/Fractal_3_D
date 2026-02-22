#pragma once
#include "Point.h"
#include <string>

using namespace std;

class Plane
{
public:
    // Конструктор по умолчанию: инициализирует нулевую плоскость
    Plane() : v1(nullptr), v2(nullptr), refPoint(nullptr),
              a(0), b(0), c(0), d(0), normal(nullptr) {}

    // Конструктор копирования со смещением d
    Plane(const Plane& pl, double dif)
        : v1(new Point(*pl.v1)), v2(new Point(*pl.v2)), refPoint(new Point(*pl.refPoint)),
          a(pl.a), b(pl.b), c(pl.c), d(pl.d - dif), normal(new Point(*pl.normal)) {}

    // Конструктор из указателя на Plane со смещением d
    Plane(const Plane* pl, double dif)
        : v1(new Point(*pl->v1)), v2(new Point(*pl->v2)), refPoint(new Point(*pl->refPoint)),
          a(pl->a), b(pl->b), c(pl->c), d(pl->d - dif), normal(new Point(*pl->normal)) {}

    // Конструктор по трём точкам: строит плоскость через A, B, C
    Plane(const Point& A, const Point& B, const Point& C)
        : refPoint(new Point(A))
    {
        Point edge1 = B - A;
        Point edge2 = C - A;

        v1 = new Point(edge1); v1->norm();
        v2 = new Point(edge2); v2->norm();

        normal = new Point(edge1.cross(edge2));
        normal->norm();

        a = normal->x;
        b = normal->y;
        c = normal->z;
        d = -normal->dot(*refPoint);

    }

    // Конструктор по указателям на три точки
    Plane(const Point* A, const Point* B, const Point* C)
        : Plane(*A, *B, *C) {}

    // Деструктор: освобождает выделенную память
    ~Plane()
    {
        delete v1;
        delete v2;
        delete normal;
        delete refPoint;
    }

    // Вычисляет расстояние от точки до плоскости (со знаком)
    double Distance(const Point& p) const
    {
        return a * p.x + b * p.y + c * p.z + d;
    }

    // Проверяет, лежит ли точка в пределах погрешности eps на плоскости
    bool In_plane(const Point& p) const
    {
        return std::abs(Distance(p)) <= eps;
    }

    // Оператор присваивания
    Plane& operator=(const Plane& rhs)
    {
        if (this != &rhs)
        {
            // Удаляем старые данные
            delete v1;
            delete v2;
            delete normal;
            delete refPoint;

            // Копируем новые
            v1       = new Point(*rhs.v1);
            v2       = new Point(*rhs.v2);
            refPoint = new Point(*rhs.refPoint);
            normal   = new Point(*rhs.normal);
            a = rhs.a; b = rhs.b; c = rhs.c; d = rhs.d;
        }
        return *this;
    }

    // Публичный доступ только для чтения к геометрическим данным
    const Point& getV1()       const { return *v1; }
    const Point& getV2()       const { return *v2; }
    const Point& getNormal()   const { return *normal; }
    const Point& getRefPoint() const { return *refPoint; }
    double getA() const { return a; }
    double getB() const { return b; }
    double getC() const { return c; }
    double getD() const { return d; }

private:
    Point* v1;        // нормализованный первый ребёрный вектор
    Point* v2;        // нормализованный второй ребёрный вектор
    Point* normal;    // нормализованный вектор нормали
    Point* refPoint;  // опорная точка, лежащая на плоскости
    double a, b, c, d; // коэффициенты уравнения плоскости: ax + by + cz + d = 0
    static constexpr double eps = 1e-6;

    friend std::ostream& operator<<(std::ostream& os, const Plane& pl)
    {
        os << pl.a << "x + (" << pl.b << ")y + (" << pl.c << ")z + (" << pl.d << ") = 0\n";
        return os;
    }
};
