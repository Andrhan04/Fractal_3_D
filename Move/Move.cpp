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
#include "..\Templates\MyWriter.h"

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

void programm(int steps, int conf_id, int exp_id) {
	vector<Particle> particles;
	input(conf_id, particles);
	if(particles.size() == 0) {
		cerr << "No particles to simulate." << endl;
		return;
	}
	int mem_out_bufer = -1;
	for (int step = 0; step < steps; step++) {
		bool out = true;
		for (size_t i = 0; i < particles.size(); i++) {
			particles[i].step();
			out = out && particles[i].check_in_work();
		}
		if (step % (steps / 100) == 0) {
			cout << "Complete: " << step / (steps / 100) << endl;
		}
		if (out && mem_out_bufer < 0) {
			mem_out_bufer = step;
		}
	}
	MyWriter writer;
	int id = writer.write_result_experiment(particles);

	json j;
	j["conf_id"] = conf_id;
	j["experiment_id"] = exp_id;
	j["steps"] = steps;
	j["particles_count"] = particles.size();
	j["result"] = id;
	j["out_bufer"] = mem_out_bufer;
	writer.write_result_experiment_config(j, exp_id);

	for (auto& i : particles) {
		if (!i.check()) {
			cout << "ERROR" << i.position;
		}
	}
}

void check_input(int conf_id) {
	vector<Particle> particles;
	input(conf_id, particles);
	for (auto& i : particles) {
		if (!i.check()) {
			cout << "ERROR" << i.position;
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
			int exp_id = value["exp_id"]; // ID эксперимента для сохранения результатов
			cout << "Configuration ID: " << conf_id << ", Repeat: " << repeat << ", Steps: " << steps << endl;
			//check_input(conf_id);
			for (int i = 0; i < repeat; i++) {
				cout << "Run " << i + 1 << " of configuration " << conf_id << ":\n";
				programm(steps, conf_id, exp_id);
			}
		}
		return 0;
	}
	catch (const exception& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
}
