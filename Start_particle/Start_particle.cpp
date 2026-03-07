#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "..\Templates\Bufer.h"
#include "..\Templates\Point.h" 
#include "..\Templates\json.hpp"
#include "..\Templates\MyReader.h"
#include "..\Templates\MyWriter.h"

using namespace std;
using json = nlohmann::json;

// Генерируем заданное количество частиц в буферной зоне
int generate_particle(int n, int sz, int id_buf) {
	MyReader reader;
	BuferZone* buf = reader.read_bufer(id_buf, sz);
    cout << *buf;
    cout << "Buffer zone created" << endl;

    // Генерируем частицы
    vector<Point> particles;
    int attempts = 0;      // Счётчик попыток
    int outsideCount = 0;  // Счётчик точек вне зоны

    while (particles.size() < static_cast<size_t>(n)) {
        Point p = buf->generate_point();
        bool duplicate = false;

        // Проверяем на дубликаты
        for (const Point& saved : particles) {
            if (saved == p) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate && buf->In_Figure(p)) {
            particles.push_back(p);
        }
        else {
            if (!duplicate) {
                ++outsideCount;
            }
            ++attempts;
        }

        // Прерываем, если слишком много попыток
        if (attempts == n * 4 && particles.size() * 3 < static_cast<size_t>(n)) {
            if (outsideCount > 10) {
                cout << "Generated outside zone: " << outsideCount << endl;
            }
            throw runtime_error("Failed to generate the required number of particles");
        }
    }

    // Сохраняем частицы в файл
	MyWriter writer;
	int id = writer.write_particles(particles);
    return id;
}

int main() {
    // Получаем свободные ID для конфигурации и частиц
	MyWriter writer;
    MyReader reader;

	json conf = reader.GeneratePoint();

    cout << "Input is valid" << endl;

    try {
        json j;
        j["Bufer"] = conf["Bufer"];
        j["Size"] = conf["Size"];
        j["Point"] = generate_particle(conf["N"], conf["Size"], conf["Bufer"]);
        for (auto& item : conf["Fractals"]) {
            j["Fractal"] = item;
		    writer.write_config(j);
		}
        return 0;
    }
    catch (const exception& ex) {
        cout << "Error during generation: " << ex.what() << endl;
        return 3;
    }

    cout << "Error in generation function" << endl;
    return 3;
}
