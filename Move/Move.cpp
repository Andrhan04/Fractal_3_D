#include <iostream>
#include <string>
#include <vector>
#include <omp.h>
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
void input(int conf_id, vector<Particle>& particles, int trap_id) {
	MyReader reader;
    json j = reader.read_config(conf_id);
    // cout << j.dump(4) << '\n';
    // Проверка обязательных полей
    if (!j.contains("Fractal") || !j.contains("Bufer") || !j.contains("Size")) {
		throw runtime_error("Invalid JSON configuration: missing required fields");
    }
	try {
		double size = j["Size"];
		BuferZone* bufer = reader.read_bufer(j["Bufer"], size);
		//cout << "Bufer:\n" << *bufer << endl;
		Fractal* fractal = reader.read_fractal(j["Fractal"], size);
		//cout << "Fractal:\n" << *fractal << endl;
		if (j.contains("Traps")) {
			for(auto & trap : j["Traps"]) {
				//cout << trap << endl;
				//reader.add_trap(trap, fractal);
			}
			reader.read_traps(j["Traps"][trap_id], fractal);
		}
		particles = reader.read_particles(j["Point"], bufer, fractal);
	}
	catch (const exception& e) {
		cerr << "Error during reading data: " << e.what() << endl;
		return;
	}
}

json programm(json& j, int trap_id) {
	vector<Particle> particles;
	int conf_id = j["conf_id"]; // ID конфигурации
	int steps = j["steps"]; // сколько шагов сделают частицы
	input(conf_id, particles, trap_id);
	if(particles.size() == 0) {
		cerr << "No particles to simulate." << endl;
		throw runtime_error("No particles to simulate");
	}
	MyWriter* writer = new MyWriter();
	int try_id = j["result"];
	int mem_out_bufer = -1;
	
	for (int step = 0; step < steps; step++) {
		bool out = true;
		for (size_t i = 0; i < particles.size(); i++) {
			particles[i].step();
			out = out && particles[i].check_in_work();
		}
		if (out && mem_out_bufer < 0) {
			mem_out_bufer = step;
		}
		if (step % 100000 == 0 && step != 0) {
			writer->write_step(particles, try_id, step);
		}
	}
	writer->write_result_experiment(particles, try_id);

	j["particles_count"] = particles.size();
	j["out_bufer"] = mem_out_bufer;

	for (auto& i : particles) {
		if (!i.check()) {
			cout << "ERROR" << i.position;
		}
	}

	return j;
}

void check_input(int conf_id) {
	vector<Particle> particles;
	input(conf_id, particles, 0);
	if (particles.empty()) cout << "ERROR" << endl;
	else {
		//particles[0].out_traps();
	}
	Point p = Point(3.7, 1.5, 14.3);
	particles[0].move(p);
	cout << particles[0].is_Alive();
	particles[1].out_traps();
	//0 3.6751 1.03718 14.3653 0.5
}

int main() {
	MyReader reader;
	MyWriter writer;
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
			/*
			*/
			vector<json> results(repeat);

			for(int i = 0; i < repeat; i++) {
				json j;
				j["conf_id"] = conf_id;
				j["experiment_id"] = exp_id;
				j["result"] = writer.get_new_id_try();
				j["steps"] = steps;
				results[i] = j;
			}
			if (j.contains("Traps")) {
				for (int trap_id = 0; trap_id < j["Traps"].size(); trap_id++) {
					#pragma omp parallel for
					for (int i = 0; i < repeat; i++) {
						cout << "Run " << i + 1 << " of configuration " << conf_id << ":\n";
						results[i] = programm(results[i], trap_id);
					}
				}
			}
			else {
				#pragma omp parallel for
				for (int i = 0; i < repeat; i++) {
					cout << "Run " << i + 1 << " of configuration " << conf_id << ":\n";
					results[i] = programm(results[i], -1);
				}
			}

			for(auto & res : results) {
				writer.write_result_experiment_config(res, exp_id);
			}
		}
		return 0;
	}
	catch (const exception& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
}
