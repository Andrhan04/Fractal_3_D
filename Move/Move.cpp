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

json programm(json& j) {
	vector<Particle> particles;
	int conf_id = j["conf_id"]; // ID конфигурации
	int steps = j["steps"]; // сколько шагов сделают частицы
	input(conf_id, particles);
	if(particles.size() == 0) {
		cerr << "No particles to simulate." << endl;
		throw runtime_error("No particles to simulate");
	}
	MyWriter* writer = new MyWriter();
	int try_id = j["result"];
	int mem_out_bufer = -1;
	
	// Прогресс-бар для шагов
	const int update_interval = max(1, steps / 100); // Обновляем каждые 1%
	
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
		
		// Обновление прогресс-бара
		if (step % update_interval == 0 || step == steps - 1) {
			float progress = (static_cast<float>(step + 1) / steps) * 100.0f;
			int bar_width = 50;
			int pos = static_cast<int>(bar_width * progress / 100.0f);
			
			cout << "\rStep progress: [";
			for (int i = 0; i < bar_width; ++i) {
				if (i < pos) cout << "=";
				else if (i == pos) cout << ">";
				else cout << " ";
			}
			cout << "] " << fixed << setprecision(1) << progress << "%" << flush;
		}
	}
	cout << endl; // Завершаем строку прогресса
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
	input(conf_id, particles);
	for (auto& i : particles) {
		if (!i.check()) {
			cout << "ERROR" << i.position;
		}
	}
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
			vector<json> results(repeat);

			for(int i = 0; i < repeat; i++) {
				json j;
				j["conf_id"] = conf_id;
				j["experiment_id"] = exp_id;
				j["result"] = writer.get_new_id_try();
				j["steps"] = steps;
				results[i] = j;
			}

			#pragma omp parallel for
			for (int i = 0; i < repeat; i++) {
				cout << "Run " << i + 1 << " of configuration " << conf_id << ":\n";
				results[i] = programm(results[i]);
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
