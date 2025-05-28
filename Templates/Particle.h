#pragma once
#include <random>
#include "Fractal.h"
#include "Bufer.h"
#include "Point.h"

class Particle{
public:
    Particle() {
        std::random_device device; 
        random_generator_.seed(device());
        buf = BuferZone();
        p = Point(returnRandom(1, 50), returnRandom(1, 50), returnRandom(1, 99));
        speed = 1;
        Angle_gorizontal = 1;
        Angle_vertical = 0;
    }
    Point p;
    void step() {

    }
private:
    BuferZone buf;
    double Angle_gorizontal,Angle_vertical, speed;
    std::mt19937 random_generator_;
    int returnRandom(int min, int max) {
        if (max < min) std::swap(max, min);
        std::uniform_int_distribution<int> range(min, max);
        return range(random_generator_);
    }
    

};
