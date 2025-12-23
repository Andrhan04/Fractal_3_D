#include <iostream>
#include <fstream>
#include <string>
#include "..\Templates\Fractal.h"
#include "..\Templates\Bufer.h"
#include "..\Templates\Figure.h"
#include "..\Templates\Point.h"
#include "..\Templates\json.hpp"


using json = nlohmann::json;
using namespace std;
string yes = "YES", no = "NO";
string path_to_config = "..\\Sowing\\Config\\";

bool input_pole(int pole_id, int bufer_id, double size_frac) {
    string path_to_file_fractal = "..\\Sowing\\Fractal\\Pole_" + to_string(pole_id) + ".txt";
    ifstream file_fractal(path_to_file_fractal);
    if (!file_fractal.is_open()) {
        cerr << "File whith Fractal not open\n";
        file_fractal.close();
        return false;
    }
    
    string path_to_file_bufer = "..\\Sowing\\Bufer\\Bufer_" + to_string(pole_id) + ".txt";
    ifstream file_bufer(path_to_file_bufer);
    if (!file_bufer.is_open()) {
        cerr << "File whith Bufer not open\n";
        file_fractal.close();
        file_bufer.close();
    }
    cout << "Bufer input\n";
    vector<Figure* > for_buf;
    for (double x, y, z; file_bufer >> x >> y >> z;) {
        cout << x << ' ' << y << ' ' << z << endl;
        Point *p = new Point(x, y, z);
        Figure* fig = new Figure(p, size_frac);
        for_buf.push_back(fig);
    }
    BuferZone* buf = new BuferZone(for_buf[1], for_buf[0]);
    cout << buf->get_info() << "Bufer Zone is input" << endl;

    vector<Figure* > for_frctal;
    for (double x, y, z; file_fractal >> x >> y >> z;) {
        cout << x << ' ' << y << ' ' << z << endl;
        Point* p = new Point(x, y, z);
        Figure* fig = new Figure(p, size_frac);
        for_frctal.push_back(fig);
    }

    Fractal* root = nullptr;
    Fractal* mem = nullptr;
    for (int i = 1; i < for_frctal.size(); i++) {
        Fractal* f = new Fractal(for_frctal[i-1], for_frctal[i]);
        if (root == nullptr) {
            root = f;
            mem = f;
        }
        else {
            f->prev = root;
            root->next = f;
            root = root->next;
        }
        cout << f->get_info() << endl;
    }


    file_fractal.close();
    file_bufer.close();
    return true;
}

bool input(int conf_id) {
    ifstream f(path_to_config + "Conf_" + to_string(conf_id) + ".json");
    //cout << (f.is_open() ? yes : no) << endl;
    if (f.is_open()) {
        json j;
        try{
            f >> j;
            cout << j << endl;
            input_pole(j["Fractal"], j["Bufer"], j["Size"]);


            return true;
        }
        catch (const std::exception&){
            cout << "Failed read json" << endl;
            return false;
        }
    }
    else {
        cout << "Failed input config\n" << path_to_config + "Config_" + to_string(conf_id) + ".json  " << "not open";
        return false;
    }
}


int main(){
    if (input(0)) {
        return 0;
    }
    else {
        return 1;
    }
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
