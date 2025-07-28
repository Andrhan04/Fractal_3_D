#pragma once
#include <math.h>
#include <string>

class Point
{
public:
    std::string Info() {
        return "Point: " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
    }
    Point(double X, double Y, double Z) {
        x = X;
        y = Y;
        z = Z;
    }
    Point() {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
    double x, y, z;

    void norm() {
        *this = *this / len();
    }

    double len() {
        return sqrt(x * x + y * y + z * z);
    }

    Point operator - (Point& p) {
        Point new_p = Point(x - p.x, y - p.y, z - p.z);
        return new_p;
    }
    Point operator - (Point* p) {
        Point new_p = Point(x - p->x, y - p->y, z - p->z);
        return new_p;
    }

    Point operator + (Point& p) {
        Point new_p = Point(x + p.x, y + p.y, z + p.z);
        return new_p;
    }
    Point operator + (Point* p) {
        Point new_p = Point(x + p->x, y + p->y, z + p->z);
        return new_p;
    }

    Point operator * (double deg) {
        return Point(x * deg, y * deg, z * deg);
    }
    Point operator / (double deg) {
        return Point(x / deg, y / deg, z / deg);
    }
    Point operator = (const Point& p) {
        return p;
    }

    bool operator == (const Point& p) const
    {
        return p.x == x && p.y == y && p.z == z;
    }
    bool operator == (const Point* p) const
    {
        return p->x == x && p->y == y && p->z == z;
    }

    bool operator != (const Point& p) const
    {
        return p.x != x || p.y == y || p.z == z;
    }
    bool operator != (const Point* p) const
    {
        return p->x != x || p->y == y || p->z == z;
    }
private:
    
};
