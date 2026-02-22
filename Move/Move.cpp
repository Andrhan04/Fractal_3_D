#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory> 
#include "..\Templates\json.hpp"
#include "..\Templates\MyReader.h"
#include "..\Templates\Bufer.h"
#include "..\Templates\Fractal.h"
#include "..\Templates\Particle.h"

using json = nlohmann::json;
using namespace std;

const string yes = "YES";
const string no = "NO";
const string path_to_config = "..\\Sowing\\Config\\";

// Функция читает конфигурационный JSON
void input(int conf_id, vector<Particle>& particles) {
	MyReader reader;
    json j = reader.read_config(conf_id);
    // cout << j.dump(4) << '\n';
    // Проверка обязательных полей
    if (!j.contains("Fractal") || !j.contains("Bufer") || !j.contains("Size")) {
		throw runtime_error("Invalid JSON configuration: missing required fields");
    }
	try {
		double size = j["Size"];
		Fractal* fractal = reader.read_fractal(j["Fractal"], size);
		//cout << "Fractal:\n" << *fractal << endl;
		BuferZone* bufer = reader.read_bufer(j["Bufer"], size);
		//cout << "Bufer:\n" << *bufer << endl;
		particles = reader.read_particles(j["Point"], bufer, fractal);
		/*cout << "Particles:\n";
		for (size_t i = 0; i < particles.size(); i++) {
			cout << "Particle " << i + 1 << ": " << particles[i].position;
			particles[i].check();
		}*/
	}
	catch (const exception& e) {
		cerr << "Error during reading data: " << e.what() << endl;
		return;
	}
}

void programm(int steps, int conf_id) {
	vector<Particle> particles;
	input(conf_id, particles);
	if(particles.size() == 0) {
		cerr << "No particles to simulate." << endl;
		return;
	}
	for (int step = 0; step < steps; step++) {
		for (size_t i = 0; i < particles.size(); i++) {
			particles[i].step();
		}
	}
}

int main() {
	MyReader reader;
	try {
		json j = reader.StartMove();
		for (auto item: j.items()) {
			json value = item.value();
			int conf_id = value["conf_id"]; // ID конфигурации
			int repeat = value["repeat"]; // сколько раз запускаем одну конфигурацию
			int steps = value["steps"]; // сколько шагов сделают частицы
			cout << "Configuration ID: " << conf_id << ", Repeat: " << repeat << ", Steps: " << steps << endl;
			for (int i = 0; i < repeat; i++) {
				cout << "Run " << i + 1 << " of configuration " << conf_id << ":\n";
				programm(steps, conf_id);
			}
		}
		return 0;
	}
	catch (const exception& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
}
