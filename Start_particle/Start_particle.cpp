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


using namespace std;
using namespace filesystem;
using json = nlohmann::json;


int get_new_id(string floder) {
    if (exists(floder) && is_directory(floder)) {
        int id = 0;
        for (auto i : directory_iterator(floder)) {
            string name = i.path().filename().string();
            int pos = (int)name.find('_');
            if (pos != -1) {
                int curr = 0;
                for (int i = pos + 1; name[i] != '.'; i++) {
                    curr *= 10;
                    curr += name[i] - '0';
                }
                if (id == curr) {
                    id++;
                }
            }
        }
        cout << id << endl;
        return id;
    }
    else {
        cout << floder << (!exists(floder)? " floder is not exist":" path is not directory") << endl;
        return -1;
    }
}

bool generate_particle(int n, int sz, int id_buf, int id) {
    string path_to_file_bufer = "..\\Sowing\\Bufer\\Bufer_" + to_string(id_buf) + ".txt";
    ifstream file_bufer(path_to_file_bufer);
    int mx_x = -1, mn_x = -1;
    int curr_y;
    if (!file_bufer.is_open()) {
        cerr << "File whith Bufer not open\n";
        file_bufer.close();
		throw runtime_error("File whith Bufer not open");
    }
    else {
        double buf_x1, buf_x2, buf_y1, buf_y2, buf_z1, buf_z2;
        file_bufer >> buf_x1 >> buf_y1 >> buf_z1 >> buf_x2 >> buf_y2 >> buf_z2;
        mn_x = buf_x1;
        mx_x = buf_x2;
        curr_y = buf_y1;
        cout << "Bufer input" << endl;
        file_bufer.close();

    }
    file_bufer.open(path_to_file_bufer);
    vector<Figure* > for_buf;
    for (double x, y, z; file_bufer >> x >> y >> z;) {
        Point* p = new Point(x, y, z);
        Figure* fig = new Figure(p, sz);
        for_buf.push_back(fig);
    }
    BuferZone* buf = new BuferZone(for_buf[1], for_buf[0]);
    cout << *buf;
    cout << "Bufer Zone is input" << endl;
    file_bufer.close();

    vector<Point> particles;
    int repeet = 0, blya = 0;
    while (particles.size() < n) {
        Point p = buf->generate_point();
        bool need_push = true;
        for (Point& save_p : particles) {
            if (save_p == p) {
                need_push = false;
            }
        }

        if (need_push && buf->In_Figure(p)) {
            particles.push_back(p);
        }
        else {
            if (need_push) {
                blya++;
            }
            repeet++;
        }

        if (repeet == n * 4 && particles.size() * 3 < n) {
            if (blya > 10) {
                cout << blya << " Generate" << endl;
                cout << "mn_x = " << mn_x << endl;
                cout << "mx_x = " << mx_x << endl;
                cout << "curr_y = " << curr_y << endl;
                cout << "sz = " << sz << endl;

            }
			throw runtime_error("Error in function");
        }
    }
    
    string path_to_file_save = "..\\Sowing\\Particle\\points_" + to_string(id) + ".txt";
    ofstream file(path_to_file_save);
    if (!file.is_open()) {
		throw runtime_error("File to save particles is not open");
    }
    else {
        for (Point& p : particles) {
            file << p.x << ' ' << p.y << ' ' << p.z << endl;
        }
        cout << path_to_file_save << " is creare and write point" << endl;
        file.close();
    }
    return true;
}


signed main(){
    int id_confid = get_new_id("..\\Sowing\\Config");
    int id_sowing = get_new_id("..\\Sowing\\Particle");
    if (id_confid < 0) {
        cout << "Not get new id for config" << endl;
        return 1;
    }
    if (id_sowing < 0) {
        cout << "Not get id for sowing particle" << endl;
        return 1;
    }
    string file_input = "config.txt";
    string file_config = "..\\Sowing\\Config\\Conf_" + to_string(id_confid) + ".json";
    ifstream f(file_input);
    if (f.is_open()) {
		cout << "File config is open" << endl;
        int id_buf, id_frac, n, sz;
        f >> id_buf >> id_frac >> n >> sz;
        if (n < 1 || sz < 1) {
            cout << "N and size must be posible";
        }
        else {
			cout << "Input is correct" << endl;
            if (generate_particle(n, sz, id_buf, id_sowing)) {
                ofstream js(file_config);
                json j;
                j["Config_id"] = id_confid;
                j["Bufer"] = id_buf;
                j["Fractal"] = id_frac;
                j["Size"] = sz;
                j["Point"] = id_sowing;
                js << j;
                cout << "All correct" << endl;
                return 0;
            }
            else {
                cout << "Error in generate function" << endl;
                return 3;
            }
        }
    }
    else {
        cout << "File config is not open" << endl;
        return 1;
    }
}
