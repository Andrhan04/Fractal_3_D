#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <fstream>
#include <filesystem>
#include <vector>
#include "..\Templates\Bufer.h"
#include "..\Templates\Point.h" 
#include "..\Templates\json.hpp"
#include "..\Templates\MyReader.h"

using namespace std;
using namespace filesystem;
using json = nlohmann::json;

// Получаем следующий свободный ID по именам файлов в папке
int get_new_id(const string& folder) {
    // Проверяем существование и тип пути
    if (exists(folder) && is_directory(folder)) {
        int id = 0;
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
                // Если нашли такой же ID, увеличиваем счётчик
                if (id == curr) {
                    ++id;
                }
            }
        }
        cout << "Next ID: " << id << endl;
        return id;
    }
    else {
        cout << folder << (!exists(folder) ? " folder does not exist" : " path is not a directory") << endl;
        return -1;
    }
}

// Генерируем заданное количество частиц в буферной зоне
bool generate_particle(int n, int sz, int id_buf, int id) {
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
    string path_to_file_save = "..\\Sowing\\Particle\\points_" + to_string(id) + ".txt";
    ofstream file(path_to_file_save);
    if (!file.is_open()) {
        throw runtime_error("Failed to create file for saving particles");
    }
    try {

        for (const Point& p : particles) {
            file << p.x << ' ' << p.y << ' ' << p.z << '\n';
        }
        cout << path_to_file_save << " created and points written" << endl;
        file.close();
    }
    catch (...){
        throw runtime_error("Failed saving particles");
    }
    return true;
}

int main() {
    // Получаем свободные ID для конфигурации и частиц
    int id_confid = get_new_id("..\\Sowing\\Config");
    int id_sowing = get_new_id("..\\Sowing\\Particle");

    if (id_confid < 0) {
        cout << "Failed to get ID for configuration" << endl;
        return 1;
    }
    if (id_sowing < 0) {
        cout << "Failed to get ID for particles" << endl;
        return 1;
    }

    string file_input = "config.txt";
    string file_config = "..\\Sowing\\Config\\Conf_" + to_string(id_confid) + ".json";

    ifstream f(file_input);
    if (!f.is_open()) {
        cout << "Failed to open configuration file" << endl;
        return 1;
    }

    cout << "Configuration file opened" << endl;
    int id_buf, id_frac, n, sz;
    f >> id_buf >> id_frac >> n >> sz;
    f.close();

    if (n < 1 || sz < 1) {
        cout << "N and size must be positive" << endl;
        return 1;
    }

    cout << "Input is valid" << endl;

    try {
        if (generate_particle(n, sz, id_buf, id_sowing)) {
            ofstream js(file_config);
            json j;
            j["Config_id"] = id_confid;
            j["Bufer"] = id_buf;
            j["Fractal"] = id_frac;
            j["Size"] = sz;
            j["Point"] = id_sowing;
            js << j;
            cout << "Everything completed successfully" << endl;
            return 0;
        }
    }
    catch (const exception& ex) {
        cout << "Error during generation: " << ex.what() << endl;
        return 3;
    }

    cout << "Error in generation function" << endl;
    return 3;
}
