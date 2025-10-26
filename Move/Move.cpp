#include <iostream>
#include <fstream>
#include <string>
#include "..\Templates\Fractal.h"
#include "..\Templates\Figure.h"
#include "..\Templates\Point.h"



using namespace std;
string yes = "YES", no = "NO";
string path_to_config = "..\\Sowing\\Config\\";

void input_pole(int conf_id) {
    ofstream f(path_to_config + "Config_" + to_string(conf_id) + ".txt");
    cout << (f.is_open() ? yes : no) << endl;
}


int main(){
    input_pole(0);
/*
// Test Fractal
    cout << "-------------------------------------------------------------\n\tFractal\n" << endl;
    Fractal frac = Fractal();
    cout << frac.get_info() << endl;
    cout << "----------------------------------------------------------" << "\nPoint Chek\n";
    Point *p = new Point(0, 0, -100);
    cout << "CREATE " << p->Info() << endl;
    cout << (frac.In_Figure(*p) ? yes : no) << endl;
*/
    return 0;
}
