#include <iostream>
#include <vector>
#include "..\Templates\Trap.h"
#include "..\Templates\Fractal.h"
#include "..\Templates\MyWriter.h"
#include "..\Templates\MyReader.h"
#include "..\Templates\Particle.h"
#include "..\Templates\json.hpp"

using namespace std;
using json = nlohmann::json;

int main(){
    MyReader reader;
	json trap_config = reader.read_to_create_trap();

    cout << trap_config;
    return 0;
}
