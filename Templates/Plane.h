#pragma once
#include "Point.h"
#include <string>

using namespace std;

class Plane
{
public:
    Plane() : v1(), v2(), refPoint(),
              a(0), b(0), c(0), d(0), normal() {}

    Plane(const Plane& pl, double dif)
        : v1(pl.v1), v2(pl.v2), refPoint(pl.refPoint),
          a(pl.a), b(pl.b), c(pl.c), d(pl.d - dif), normal(pl.normal) {}

    Plane(const Plane* pl, double dif)
        : v1(pl->v1), v2(pl->v2), refPoint(pl->refPoint),
          a(pl->a), b(pl->b), c(pl->c), d(pl->d - dif), normal(pl->normal) {}

    Plane(const Point& A, const Point& B, const Point& C)
        : refPoint(A)
    {
        Point edge1 = B - A;
        Point edge2 = C - A;

        v1 = edge1; v1.norm();
        v2 = edge2; v2.norm();

        normal = edge1.cross(edge2);
        normal.norm();

        a = normal.x;
        b = normal.y;
        c = normal.z;

        d = -normal.dot(refPoint);
    }

    Plane(const Point* A, const Point* B, const Point* C)
        : Plane(*A, *B, *C) {}

    double Distance(const Point& p) const
    {
        return a * p.x + b * p.y + c * p.z + d;
    }

    bool In_plane(const Point& p) const
    {
        return std::abs(Distance(p)) <= eps;
    }

    // Оператор присваивания
    Plane& operator=(const Plane& rhs)
    {
        if (this != &rhs)
        {
            v1       = rhs.v1;
            v2       = rhs.v2;
            refPoint = rhs.refPoint;
            normal   = rhs.normal;
            a = rhs.a; b = rhs.b; c = rhs.c; d = rhs.d;
        }
        return *this;
    }

    // Публичный доступ только для чтения к геометрическим данным
    const Point& getV1()       const { return v1; }
    const Point& getV2()       const { return v2; }
    const Point& getNormal()   const { return normal; }
    const Point& getRefPoint() const { return refPoint; }
    double getA() const { return a; }
    double getB() const { return b; }
    double getC() const { return c; }
    double getD() const { return d; }

private:
    Point v1;        // нормализованный первый реберный вектор
    Point v2;        // нормализованный второй реберный вектор
    Point normal;    // нормализованный вектор нормали
    Point refPoint;  // опорная точка, лежащая на плоскости
    double a, b, c, d; // коэффициенты уравнения плоскости: ax + by + cz + d = 0
    static constexpr double eps = 1e-6;

    friend std::ostream& operator<<(std::ostream& os, const Plane& pl)
    {
        os << pl.a << "x + (" << pl.b << ")y + (" << pl.c << ")z + (" << pl.d << ") = 0\n";
        return os;
    }
};
