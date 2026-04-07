#pragma once
#include <cmath>
#include "../Templates/Bufer.h"
#include "../Templates/Point.h"
#include "../Templates/Fractal.h"
#include "../Templates/Generator.h"

// Убрали using namespace std; во избежание загрязнения глобального пространства имёв
class Particle {
public:
    // Конструктор по умолчанию (заглушка)
    Particle()
        : buf(nullptr) // создаём временный объект и сразу инициализируем ссылку
        , root(nullptr)  // аналогично для root, если требуется ссылка на Fractal
        , speed(1.0)
        , angleHorizontal(1.0)
        , angleVertical(0.0)
        , work(false)
        , Alive(true)
    {
        position = Point(
            static_cast<double>(generator_.get_int(1, 50)),
            static_cast<double>(generator_.get_int(1, 50)),
            static_cast<double>(generator_.get_int(1, 99))
        );
    }

    // Конструктор для генерации и посева
    Particle(const Point& p, BuferZone& b, Fractal& f)
        : buf(& b)
        , root(& f)
        , position(p)
        , speed(1.0)
        , angleHorizontal(0.0)
        , angleVertical(0.0)
        , work(false)
        , Alive(true)
    {
    }

    Particle(const Point& p, BuferZone* b, Fractal* f)
        : buf(b)
        , root(f)
        , position(p)
        , speed(1.0)
        , angleHorizontal(0.0)
        , angleVertical(0.0)
        , work(false)
        , Alive(true)
    {
    }

    // Запрещаем копирование, чтобы не дублировать ссылки
    Particle(const Particle&) = delete;
    Particle& operator=(const Particle&) = delete;

    // Разрешаем перемещение (опционально)
    Particle(Particle&& other) noexcept
        : buf(other.buf)
        , root(other.root)
        , position(std::move(other.position))
        , speed(1.0)
        , angleHorizontal(other.angleHorizontal)
        , angleVertical(other.angleVertical)
        , work(other.work)
        , Alive(true)
        //, randomGenerator_(std::move(other.randomGenerator_))
    {
        other.buf = nullptr;
        other.root = nullptr;
    }

    ~Particle() {
        // Удаляем buf, только если он был создан в конструкторе по умолчанию
        // В реальности лучше передавать владение через std::unique_ptr,
        // но в рамках задания «всё на ссылки» оставляем так
    }

    // Текущая позиция частицы (публичное поле оставлено по исходному коду)
    Point position;

    // Шаг симуляции
    void step() {
        Point delta = calculateDelta();
        if (work) {
            stepInWork(delta);
        } else {
            stepInBufer(delta);
        }
    }

    bool check_in_work() {
        return work;
	}

    bool check() {
        if (work) {
            return (root->In_Figure(position));
        }
        else {
            return(buf->In_Figure(position));
        }
    }

    void out_traps() {
        Fractal* root = this->root;
        while (root != nullptr) {
            cout << root->get_traps_count() << endl;
            root->out_traps();
            root = root->next;
        }
    }

    void move(Point& new_pos) {
        if (root->In_Figure(new_pos)) {
            position = new_pos;
            if (root->check_cath(position)) {
                Alive = false;
            }
        }
        cout << "WA";
    }

    bool is_Alive() {
        return Alive;
    }

private:
    // Вложенные объекты теперь хранятся по ссылям
    BuferZone *buf;
    Fractal   *root;

    bool Alive;
    bool work;                         // флаг: находится ли частица внутри фрактала
    double angleHorizontal;            // азимутальный угол (в градусах)
    double angleVertical;              // полярный угол (в градусах)
    double speed;                      // скорость перемещения

    static constexpr double PI = 3.14159265358979323846;
    static constexpr double EPS = 1e-6;

    Generator generator_;

    // Вычисление вектора перемещения по случайным углам и скорости
    Point calculateDelta() {
        double angleH = static_cast<double>(generator_.get_int(1, 360));
        double angleV = static_cast<double>(generator_.get_int(1, 180));
        double radH = angleH * PI / 180.0;
        double radV = angleV * PI / 180.0;

        return Point(speed * std::sin(radH) * std::sin(radV),
                     speed * std::cos(radH) * std::sin(radV),
                     speed * std::cos(radV));
    }

    // Перемещение внутри фрактала
    void stepInWork(const Point& delta) {
        Point newPos = position + delta;
        if (root->In_Figure(newPos)) {
            position = newPos;
            if (root->check_cath(position)) {
                Alive = false;
            }
        }
    }

    // Перемещение внутри буферной зоны
    void stepInBufer(const Point& delta) {
        Point newPos = position + delta;
        if (buf->In_Figure(newPos)) {
            position = newPos;
			//cout << "Particle moved in buffer to position: " << newPos;
        }
        else{
            newPos.x += 1;
            if (root->In_Figure(newPos)) {
				//cout << "Particle entered the fractal at position: " << newPos;
                position = newPos;
                work = true;
            }
        }
    }
};
