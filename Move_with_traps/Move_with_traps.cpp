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
			//cout << j["Traps"][trap_id] << endl;
			//cout << trap_id << endl;
			//for(auto & trap : j["Traps"]) {
			//	cout << trap << endl;
			//	//reader.add_trap(trap, fractal);
			//
			// }
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
	if (particles.size() == 0) {
		cerr << particles.size() << endl;
		cerr << "No particles to simulate." << endl;
		throw runtime_error("No particles to simulate");
	}
	MyWriter* writer = new MyWriter();
	int try_id = j["result"];
	int mem_out_bufer = -1;
	int time_live = (particles.size() / 2.7);
	int cnt_live = particles.size();
	j["N/e"] = time_live;
	for (int step = 0; step < steps; step++) {
		bool out = true;
		for (size_t i = 0; i < cnt_live; i++) {
			if (particles[i].is_Alive()) {
				particles[i].step();
				out = out && particles[i].check_in_work();
			}
			if (!particles[i].is_Alive()) {
				swap(particles[i], particles[cnt_live - 1]);
				cnt_live--;
				i--;
			}
		}
		if (cnt_live <= time_live && !j.contains("time_live")) {
			j["time_live"] = step + 1;
		}
		if (out && mem_out_bufer < 0) {
			mem_out_bufer = step;
		}
		if (step % 100000 == 0 && step != 0) {
			writer->write_step_trap(particles, try_id, step);
		}
	}

	j["particles_count"] = particles.size();
	if (!j.contains("time_live")) j["time_live"] = -1;
	j["Alive"] = cnt_live;
	j["Traps_count"] = particles[0].get_traps_count();
	for (auto& i : particles) {
		if (!i.check()) {
			cout << "ERROR" << i.position;
		}
	}
	writer->write_result_experiment_trap(particles, try_id);

	return j;
}

void check_input(int conf_id) {
	vector<Particle> particles;
	input(conf_id, particles, 0);
	if (particles.empty()) cout << "ERROR" << endl;
	else {
		cout << "Input data is correct. Number of particles: " << particles.size() << endl;
		cout << particles[0].position << ' ' << particles[particles.size() - 1].position << endl;
		swap(particles[0], particles[particles.size() - 1]);
		cout << particles[0].position << ' ' << particles[particles.size() - 1].position << endl;
	}
	//Point p = Point(3.7, 1.5, 14.3);
	//particles[0].move(p);
	//cout << particles[0].is_Alive();
	//particles[1].out_traps();
	//0 3.6751 1.03718 14.3653 0.5
}

int main() {
	MyReader reader;
	MyWriter writer;
	try {
		json j = reader.StartMove();
		for (auto item : j.items()) {
			json value = item.value();
			int conf_id = value["conf_id"]; // ID конфигурации
			int repeat = value["repeat"]; // сколько раз запускаем одну конфигурацию
			int steps = value["steps"]; // сколько шагов сделают частицы
			int exp_id = value["exp_id"]; // ID эксперимента для сохранения результатов
			cout << "Configuration ID: " << conf_id << ", Repeat: " << repeat << ", Steps: " << steps << endl;
			//check_input(conf_id);
			json j_trap = reader.read_config(conf_id);
			for (int trap_id = 0; trap_id < j_trap["Traps"].size(); trap_id++) {
				cerr << "Running configuration " << conf_id << " with trap " << trap_id << "...\n";
				vector<json> results(repeat);
				for (int i = 0; i < repeat; i++) {
					json j;
					j["conf_id"] = conf_id;
					j["experiment_id"] = exp_id;
					j["steps"] = steps;
					j["result"] = writer.get_new_id_try_traps();
					j["Trap_id"] = j_trap["Traps"][trap_id];
					results[i] = j;
				}
				#pragma omp parallel for
				for (int i = 0; i < repeat; i++) {
					cout << "Run " << i + 1 << " of configuration " << conf_id << ":\n";
					results[i] = programm(results[i], trap_id);
				}
				for (auto& res : results) {
					writer.write_result_experiment_config_with_trap(res, exp_id);
				}
			}
		}
		return 0;
	}
	catch (const exception& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
}
