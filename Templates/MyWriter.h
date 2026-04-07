#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <filesystem>
#include "json.hpp"
#include "..\Templates\Bufer.h"
#include "..\Templates\Fractal.h"
#include "..\Templates\Particle.h"

using namespace std;
using namespace filesystem;
using json = nlohmann::json;

// Класс для чтения данных из файлов: конфигураций, буферов и фракталов
class MyWriter{
private:
    // Создаёт полный путь к файлу по базовому пути и ID
    void make_path(const string& folder) {
		string path = "", help = "";
        for (auto c : folder) {
            if(c == '\\') {
				path += help;
                help = "\\";
            }
            else {
                help += c;
            }
        }
		//cout << path << endl;
        if (filesystem::create_directories(path)) {
            //std::cout << "Create" << endl;
            path += help;
        }
    }

    int get_new_id(const string& path) {
        string folder = "", help = "";
        for (auto c : path) {
            if (c == '\\') {
                folder += help;
                help = "\\";
            }
            else {
                help += c;
            }
        }
        // Проверяем существование и тип пути
        if (exists(folder) && is_directory(folder)) {
            set <int> mem;
            // Проходим по всем файлам в директории
            for (const auto& entry : directory_iterator(folder)) {
                const string name = entry.path().filename().string();
                // Ищем символ '_' в имени файла
                size_t pos = name.find('_');
                if (pos != string::npos) {
                    // Извлекаем числовую часть после '_'
                    int curr = 0;
                    for (size_t i = pos + 1; i < name.size() && isdigit(name[i]); ++i) {
                        curr = curr * 10 + (name[i] - '0');
                    }
                    mem.insert(curr);
                }
            }
            int id = 0;
            for (auto i : mem) {
                if (id == i) {
                    id++;
                }
            }
            //cout << "Next ID: " << id << endl;
            return id;
        }
        else {
			throw runtime_error(folder + (!exists(folder) ? " folder does not exist" : " path is not a directory"));
        }
    }

    string path_to_file_fractal; // базовый путь к файлам фракталов
    string path_to_file_bufer;   // базовый путь к файлам буферов
    string path_to_config;       // базовый путь к конфигурационным файлам
	string path_to_particles;    // базовый путь к файлам с данными частиц
	string path_to_save_result_experiment; // базовый путь для сохранения результатов эксперимента
    string path_to_save_result_experiment_trap; // базовый путь для сохранения результатов эксперимента
	string path_to_save_result_experiment_config; // базовый путь для сохранения конфигурации эксперимента
    string path_to_save_result_experiment_config_trap; // базовый путь для сохранения конфигурации эксперимента
	string path_to_file_step;     // базовый путь для сохранения данных по шагам симуляции
    string path_to_traps;
public:
    MyWriter() {
		path_to_file_fractal = "..\\Sowing\\Fractal\\Pole_";
		path_to_file_bufer = "..\\Sowing\\Bufer\\Bufer_";
		path_to_config = "..\\Sowing\\Config\\Conf_";
		path_to_particles = "..\\Sowing\\Particle\\points_";
		path_to_save_result_experiment = "..\\Experiments\\exp_";
        path_to_save_result_experiment_trap = "..\\Experiments_traps\\exp_";
		path_to_save_result_experiment_config = "..\\Results\\experiment_";
        path_to_save_result_experiment_config_trap = "..\\Results_traps\\experiment_";
		path_to_file_step = "..\\Experiments\\Experiment_";
        path_to_traps = "..\\Sowing\\Trap\\Traps_";
    }

    int get_new_id_try() {
        make_path(path_to_save_result_experiment);
        int exp_id = get_new_id(path_to_save_result_experiment);
        string path = path_to_save_result_experiment + to_string(exp_id) + ".txt";
        ofstream file(path);
        if(file.is_open()) {
            file.close();
        }
        else {
            throw runtime_error("Failed to create file for saving experiment");
		}
		return exp_id;
    }

    int get_new_id_try_traps() {
        make_path(path_to_save_result_experiment_trap);
        int exp_id = get_new_id(path_to_save_result_experiment_trap);
        string path = path_to_save_result_experiment_trap + to_string(exp_id) + ".txt";
        ofstream file(path);
        if (file.is_open()) {
            file.close();
        }
        else {
            throw runtime_error("Failed to create file for saving experiment");
        }
        return exp_id;
    }

    void write_result_experiment(const vector<Particle>& particles, int exp_id) {
        string path = path_to_save_result_experiment + to_string(exp_id) + ".txt";
        ofstream file(path);
        if (!file.is_open()) {
            throw runtime_error("Failed to create file for saving experiment");
        }
        try {
            for (const Particle& p : particles) {
                file << p.position << endl;
            }
            file.close();
        }
        catch (...) {
            throw runtime_error("Failed saving experiment results");
        }
	}

    void write_result_experiment_trap(const vector<Particle>& particles, int exp_id) {
        string path = path_to_save_result_experiment_trap + to_string(exp_id) + ".txt";
        ofstream file(path);
        if (!file.is_open()) {
            throw runtime_error("Failed to create file for saving experiment");
        }
        try {
            //file << j << endl;
            for (const Particle& p : particles) {
                file << p.position << endl;
            }
            file.close();
        }
        catch (...) {
            throw runtime_error("Failed saving experiment results");
        }
    }

    void write_result_experiment_config(const json& config, int exp_id) {
        // Формируем полный путь к файлу конфигурации эксперимента по ID
		make_path(path_to_save_result_experiment_config);
        string path = path_to_save_result_experiment_config + to_string(exp_id) + ".json";
        // Проверяем, не существует ли уже файл с таким именем
        if (filesystem::exists(path)) {
			json existing_config;
			ifstream existing_file(path);
            if (existing_file.is_open()) {
                existing_file >> existing_config;
                existing_file.close();
                ofstream file(path);
                if (!file.is_open()) {
                    throw runtime_error("Failed to open existing experiment configuration file for appending");
				}
                else {
                // Добавляем новую конфигурацию к существующему массиву
                    existing_config.push_back(config);
                    file << existing_config.dump(4);
                    file.close();
                }
            }
            else {
                throw runtime_error("Failed to open existing experiment configuration file");
			}
        }
        else {
            // Создаём и открываем файл для записи
            ofstream file(path);
            if (!file.is_open()) {
                throw runtime_error("Failed to create file for saving experiment configuration");
            }
            try {
                // Записываем JSON в файл с отступами (4 пробела) для удобства чтения
                file << json::array({config}).dump(4);
                file.close();
            }
            catch (...) {
                throw runtime_error("Failed saving experiment configuration");
            }
        }
    }

    void write_result_experiment_config_with_trap(const json& config, int exp_id) {
        // Формируем полный путь к файлу конфигурации эксперимента по ID
        make_path(path_to_save_result_experiment_config_trap);
        string path = path_to_save_result_experiment_config_trap + to_string(exp_id) + ".json";
        // Проверяем, не существует ли уже файл с таким именем
        if (filesystem::exists(path)) {
            json existing_config;
            ifstream existing_file(path);
            if (existing_file.is_open()) {
                existing_file >> existing_config;
                existing_file.close();
                ofstream file(path);
                if (!file.is_open()) {
                    throw runtime_error("Failed to open existing experiment configuration file for appending");
                }
                else {
                    // Добавляем новую конфигурацию к существующему массиву
                    existing_config.push_back(config);
                    file << existing_config.dump(4);
                    file.close();
                }
            }
            else {
                throw runtime_error("Failed to open existing experiment configuration file");
            }
        }
        else {
            // Создаём и открываем файл для записи
            ofstream file(path);
            if (!file.is_open()) {
                throw runtime_error("Failed to create file for saving experiment configuration");
            }
            try {
                // Записываем JSON в файл с отступами (4 пробела) для удобства чтения
                file << json::array({ config }).dump(4);
                file.close();
            }
            catch (...) {
                throw runtime_error("Failed saving experiment configuration");
            }
        }
    }

    int write_traps(const vector<Trap>& traps) {
        int conf_id = get_new_id(path_to_traps);
        string path = path_to_traps + to_string(conf_id) + ".txt";
        ofstream file(path);
        if (!file.is_open()) {
            throw runtime_error("Failed to create file for saving traps");
        }
        try {
            for (const Trap& p : traps) {
                file << p << '\n';
            }
            file.close();
            return conf_id;
        }
        catch (...) {
            throw runtime_error("Failed saving traps");
        }
    }

	int write_particles(const vector<Point>& particles) {
	    int conf_id = get_new_id(path_to_particles);
	    string path = path_to_particles + to_string(conf_id) + ".txt";
	    ofstream file(path);
        if (!file.is_open()) {
            throw runtime_error("Failed to create file for saving particles");
        }
        try {
            for (const Point& p : particles) {
                file << p.x << ' ' << p.y << ' ' << p.z << '\n';
            }
            file.close();
            return conf_id;
        }
	    catch (...) {
            throw runtime_error("Failed saving particles");
        }
	}

    void rewrite_config(const json& config, int id) {
        string path = path_to_config + to_string(id) + ".json";
        ofstream file(path);
        if (!file.is_open()) {
            throw runtime_error("Failed to create file for saving configuration");
        }
        try {
            file << config.dump(); // Сохраняем JSON без отступов (массивы в одной строке)
            file.close();
        }
        catch (...) {
            throw runtime_error("Failed saving configuration");
        }
    }

    void write_config(const json& config) {
		int conf_id = get_new_id(path_to_config);
        string path = path_to_config + to_string(conf_id) + ".json";
        ofstream file(path);
        if (!file.is_open()) {
            throw runtime_error("Failed to create file for saving configuration");
        }
        try {
            file << config.dump(4); // Сохраняем JSON с отступами для читаемости
            file.close();
        }
        catch (...) {
            throw runtime_error("Failed saving configuration");
        }
	}

    void write_step(const vector<Particle>& particles, int exp_id, int step) {
		string path = path_to_file_step + to_string(exp_id) + "\\step_" + to_string(step) + ".txt";
		make_path(path);
        ofstream file(path);
        if (!file.is_open()) {
            throw runtime_error("Failed to create file for saving step data");
        }
        try {
            for (const Particle& p : particles) {
                file << p.position;
            }
            file.close();
        }
        catch (...) {
            throw runtime_error("Failed saving step data");
		}
    }


    void check() {
		cout << "MyReader check" << endl;
		make_path(path_to_particles);
    }
};