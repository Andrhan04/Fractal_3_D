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

class Generator {
private:
    std::mt19937 random_generator_;
    int returnRandom(int min, int max) {
        if (max < min) std::swap(max, min);
        std::uniform_int_distribution<int> range(min, max);
        return range(random_generator_);
    }
    bool chek(double x, double y, double sz) {
        return x * x + y * y <= sz * sz;
    }
    double get(double x, double sz) {
        return sz * sz - x * x;
    }
public:
    Generator() {
        std::random_device device;
        random_generator_.seed(device());
    }
    int get_int(int mn, int mx) {
        return returnRandom(std::min(mn, mx), std::max(mx, mn));
    }

    Point get_particle(int mn_x, int mx_x, int y, int sz) {
        Point p = Point();
        p.x = returnRandom(mn_x, mx_x);
        double new_y = returnRandom(-sz, sz);
        double new_z = get(new_y, sz) * (returnRandom(0, 1000) & 1 ? 1 : -1);
        p.y = y + sz;
        p.z = new_z;
        return p;
    }
};


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
        return false;
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
        cout << x << ' ' << y << ' ' << z << endl;
        Point* p = new Point(x, y, z);
        Figure* fig = new Figure(p, sz);
        for_buf.push_back(fig);
    }
    BuferZone* buf = new BuferZone(for_buf[1], for_buf[0]);
    cout << buf->get_info() << "Bufer Zone is input" << endl;
    file_bufer.close();

    vector<Point> particles;
    Generator g = Generator();
    int repeet = 0, blya = 0;
    while (particles.size() < n) {
        Point p = g.get_particle(mn_x, mx_x, curr_y, sz);
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
            }
            cout << "Error in function" << endl;
            return false;
        }
    }
    
    string path_to_file_save = "..\\Sowing\\Particle\\points_" + to_string(id) + ".txt";
    ofstream file(path_to_file_save);
    if (!file.is_open()) {
        cerr << "File to save particles is not open" << endl;
        return false;
    }
    else {
        for (Point& p : particles) {
            file << p.x << ' ' << p.y << ' ' << p.z << endl;
        }
        file.close();
        return true;
    }
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
        int id_buf, id_frac, n, sz;
        f >> id_buf >> id_frac >> n >> sz;
        cout << id_buf << ' ' << n << endl;
        if (n < 1 || sz < 1) {
            cout << "N and size must be posible";
        }
        else {
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
