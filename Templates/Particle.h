#pragma once
#include <random>
#include <math.h>
#include "Bufer.h"
#include "Point.h"

class Particle{
public:
    Particle() {
        std::random_device device; 
        random_generator_.seed(device());
        buf = new BuferZone();
        position = Point(returnRandom(1, 50), returnRandom(1, 50), returnRandom(1, 99));
        speed = 1;
        Angle_gorizontal = 1;
        Angle_vertical = 0;
        Work = false;
    }
    Particle(Point p, BuferZone *b) {
        std::random_device device;
        random_generator_.seed(device());
        buf = b;
        position = p;
        speed = 1;
        Angle_gorizontal = 1;
        Angle_vertical = 0;
        Work = false;
    }
    Point position;
    void step() {
        Angle_gorizontal = returnRandom(1, 360);
        Angle_vertical = returnRandom(1, 180);
        if (Work) {

        }
        else {

        }
    }
private:
    BuferZone *buf;
    bool Work;
    double Angle_gorizontal,Angle_vertical, speed;
    const double Pi = acos(0) * 2.0, eps = 1e-6;
    std::mt19937 random_generator_;
    
    int returnRandom(int min, int max) {
        // min,max - включает
        if (max < min) std::swap(max, min);
        std::uniform_int_distribution<int> range(min, max);
        return range(random_generator_);
    }
    
    Point Dif() {
        Point p = Point(
            speed * sin(Angle_gorizontal * Pi) * sin(Angle_vertical * Pi),
            speed * cos(Angle_gorizontal * Pi) * sin(Angle_vertical * Pi),
            speed * cos(Angle_vertical * Pi)
        );
        return p;
    }

    void StepInWork(Point& p) {
        position = position + p;
    }

    void StepInBufer(Point& dif) {
        Point new_pos = position + dif;
        if (buf->In_Figure(new_pos)) {
            position = new_pos;
        }
    }
};
