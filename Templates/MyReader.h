#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"
#include "..\Templates\Bufer.h"
#include "..\Templates\Fractal.h"
#include "..\Templates\Particle.h"
#include "..\Templates\Trap.h"

using json = nlohmann::json;
using namespace std;


// Класс для чтения данных из файлов: конфигураций, буферов и фракталов
class MyReader{
private:

    void add_trap(Trap* t, Fractal* root) {
        int seg = t->segment_id;
        while (root != nullptr && seg > 0) {
            root = root->next;
            seg--;
		}
        root->addTrap(t);
        while(root->prev != nullptr) {
            root = root->prev;
		}
	}

    string path_to_file_fractal; // базовый путь к файлам фракталов
    string path_to_file_bufer;   // базовый путь к файлам буферов
    string path_to_config;       // базовый путь к конфигурационным файлам
	string path_to_trap;  // базовый путь к файлам конфигураций для ловушки
	string path_to_particles;    // базовый путь к файлам с данными частиц
public:
    // Конструктор по умолчанию: инициализирует пути к каталогам данных
    MyReader() {
		path_to_file_fractal = "..\\Sowing\\Fractal\\Pole_";
		path_to_file_bufer = "..\\Sowing\\Bufer\\Bufer_";
		path_to_config = "..\\Sowing\\Config\\Conf_";
		path_to_particles = "..\\Sowing\\Particle\\points_";
		path_to_trap = "..\\Sowing\\Trap\\Traps_";
    }

    json read_to_create_trap() {
        string path = "input.json";
		ifstream file(path);
        if (!file.is_open()) {
            throw runtime_error("File not is open " + path);
        }
		json j;
		file >> j;
        file.close();
        if (!j.contains("N_min") || !j.contains("conf_id") || !j.contains("N_max") || !j.contains("Step"))
			throw runtime_error("Invalid JSON configuration: missing required fields");
        return j;
    }

    json StartMove() {
		string path = "input.json";
		ifstream file(path);
        if (!file.is_open()) {
            throw runtime_error("File not is open " + path);
		}
		json j;
		file >> j;
		file.close();
        for (auto item: j.items()) {
			json value = item.value();
            if (!value.contains("conf_id") || !value.contains("repeat") || !value.contains("steps")) {
                throw runtime_error("Invalid JSON configuration: missing required fields in " + item.key());
			}
		}
		return j;
    }

    json GeneratePoint() {
        string path = "input.json";
        ifstream file(path);
        if (!file.is_open()) {
            throw runtime_error("File not is open " + path);
        }
		json j;
        file >> j;
        file.close();
        if (!j.contains("Bufer") || !j.contains("Fractals") || !j.contains("N") || !j.contains("Size")) {
            throw runtime_error("File not is open " + path);
        }
        if (j["N"] > 0 && j["Size"] > 0) {
            if (j["Fractals"].size() == 0) {
                throw runtime_error("Invalid JSON configuration: missing required fields in Fractals array in " + path);
            }
            else {
                return j;
			}
        }
        else {
            throw runtime_error("N and Size must be greater than 0 in " + path);
		}
    }

    // Читает JSON-конфигурацию по заданному идентификатору
    json read_config(int config_id) {
        // Формируем полный путь к файлу конфигурации
        string path = path_to_config + to_string(config_id) + ".json";
        ifstream file(path);
        if (!file.is_open()) {
			throw runtime_error("File not is open " + path);
        }
        json j;
        file >> j;
		file.close();
        // Проверка обязательных полей
        if (!j.contains("Fractal") || !j.contains("Bufer") || !j.contains("Size")) {
            throw runtime_error("Invalid JSON configuration: missing required fields");
        }
        return j;
	}

    void read_traps(int config_id, Fractal* root) {
        string path = path_to_trap + to_string(config_id) + ".txt";
        ifstream file(path);
        if (!file.is_open()) {
            throw runtime_error("File not is open " + path);
        }
        int id;
		double x, y, z, range;
        while (file >> id >> x >> y >> z >> range) {
			Point* p = new Point(x, y, z);
			Trap* t = new Trap(id, p, range);
            //cout << *t << endl;
			add_trap(t, root);
        }
    }

    // Читает данные буфера из текстового файла и возвращает указатель на BuferZone
    BuferZone* read_bufer(int bufer_id, int size) {
        // Формируем полный путь к файлу буфера
        string path = path_to_file_bufer + to_string(bufer_id) + ".txt";
        ifstream file(path);
        if (!file.is_open()) {
            throw runtime_error("File not is open " + path);
        }
        // вектор shared_ptr<Figure> для хранения точек в BuferZone
        vector<shared_ptr<Figure>> for_buf;
        double x, y, z;
        // Считываем координаты точек из файла
        while (file >> x >> y >> z) {
            // создаём объект точки и добавляем в вектор
            Point p(x, y, z);
            for_buf.push_back(std::make_shared<Figure>(p, size));
        }

        // проверяем количество точек в буфере
        if (for_buf.size() != 2) {
			throw runtime_error("Buffer must continue 2 point");
        }

        // создаём объект буфера из двух точек
        BuferZone* bufer = new BuferZone(for_buf[1], for_buf[0]);
        return bufer;
	}

    // Читает данные фрактала из текстового файла и строит двусвязный список фракталов
    Fractal* read_fractal(int pole_id, int size) {
        // Формируем полный путь к файлу фрактала
        string path = path_to_file_fractal + to_string(pole_id) + ".txt";
        ifstream file(path);
        if (!file.is_open()) {
            throw runtime_error("File not is open " + path);
        }
        // вектор фракталов
        vector<shared_ptr<Figure>> for_fractal;
        double x, y, z;
        // Считываем координаты точек из файла
        while (file >> x >> y >> z) {
            Point p(x, y, z);
            for_fractal.push_back(std::make_shared<Figure>(p, size));
        }
        // строим двусвязный список фракталов
        Fractal* root = nullptr;       // корень списка
        Fractal* prevFractal = nullptr; // предыдущий фрактал в списке
        // Проходим по считанным точкам и создаём фракталы, связывая их
        for (size_t i = 1; i < for_fractal.size(); ++i) {
            Fractal* f = new Fractal(for_fractal[i], for_fractal[i - 1]);
            if (!root) {
                root = f;
            }
            else {
                prevFractal->next = f;
                f->prev = prevFractal;
            }
            prevFractal = f;
            //cout << *f << '\n';
        }
        return root;
    }

    vector<Particle> read_particles(int id, BuferZone *buf, Fractal *f) {
        vector<Particle> particles;
		string path = path_to_particles + to_string(id) + ".txt";
		ifstream file(path);
        if (!file.is_open()) {
            throw runtime_error("File not is open " + path);
		}
		double x, y, z;
        while (file >> x >> y >> z) {
            Point p(x, y, z);
            particles.emplace_back(p, buf, f);
		}
		return particles;
	}
};
