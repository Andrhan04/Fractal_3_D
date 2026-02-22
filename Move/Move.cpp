#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory> 
#include "..\Templates\Fractal.h"
#include "..\Templates\Bufer.h"
#include "..\Templates\Figure.h"
#include "..\Templates\Point.h"
#include "..\Templates\json.hpp"
#include "..\Templates\MyReader.h"

using json = nlohmann::json;
using namespace std;

const string yes = "YES";
const string no = "NO";
const string path_to_config = "..\\Sowing\\Config\\";

// Функция читает конфигурационный JSON и вызывает input_pole
void input(int conf_id) {
	MyReader reader;
    json j = reader.read_config(conf_id);
    cout << j.dump(4) << '\n';
    // Проверка обязательных полей
    if (!j.contains("Fractal") || !j.contains("Bufer") || !j.contains("Size")) {
		throw runtime_error("Invalid JSON configuration: missing required fields");
    }
	double size = j["Size"];
	Fractal* fractal = reader.read_fractal(j["Fractal"], size);
	BuferZone* bufer = reader.read_bufer(j["Bufer"], size);

	cout << "Fractal:\n" << *fractal << "\nBufer:\n" << *bufer << "\nSize: " << size << endl;
}

int main() {
    input(0);

    return 0;
}
