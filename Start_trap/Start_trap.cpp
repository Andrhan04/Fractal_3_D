#include <iostream>
#include <vector>
#include "..\Templates\Trap.h"
#include "..\Templates\Fractal.h"
#include "..\Templates\MyWriter.h"
#include "..\Templates\MyReader.h"
#include "..\Templates\Particle.h"
#include "..\Templates\json.hpp"
#include "..\Templates\Generator.h"

using namespace std;
using json = nlohmann::json;

int get_count_segments_fractal(const Fractal* root) {
    if (root == nullptr) return 0;
    const Fractal* buf = root;
    int ans = 1;
    while (buf->next != nullptr) {
        buf = buf->next;
        ans++;
    }
    return ans;
}

Trap gen_posible_trap(Fractal* root, int count_segments_fractal) {
    Generator g;
    int displacement = g.get_int(0, count_segments_fractal - 1);
    Fractal* buf = root;
    int segment_id = 0;
    while (displacement--) {
        buf = buf->next;
        segment_id++;
    }
    Point* p = new Point(buf->generate_point());
    Trap create_trap = Trap(p, segment_id);
    return create_trap;
}

int Generate_traps(json& config, MyReader& reader, int trap_count) {
    Fractal* root = reader.read_fractal(config["Fractal"], config["Size"]);
    int count_segments_fractal = get_count_segments_fractal(root);
    vector<Trap> traps;
    int n = 0;
    while (n < trap_count) {
        bool need_push_trap = true;
        Trap posible_trap = gen_posible_trap(root,count_segments_fractal);
        for (int i = 0; i < n; i++) {
            if (traps[i].distance(posible_trap.position) <= 1) {
                need_push_trap = false;
            }
        }
        if (need_push_trap) {
            n++;
            traps.push_back(posible_trap);
        }
    }
    sort(traps.begin(), traps.end());
    MyWriter writer;
    int id = writer.write_traps(traps);
    cout << "Traps create with id = " << id << endl;
    return id;
}

int main(){
    MyReader reader;
    try {
	    json trap_config = reader.read_to_create_trap();
        cout << trap_config << endl;
        int conf_id = trap_config["conf_id"];
        json config = reader.read_config(conf_id);
        vector<int> traps;
        cout << config << endl;
        for (int N_trap = trap_config["N_min"]; N_trap <= trap_config["N_max"]; N_trap += trap_config["Step"]) {
            int trap_id = Generate_traps(config, reader, N_trap);
            traps.push_back(trap_id);
        }
        config["Traps"] = traps;
        MyWriter writer;
        writer.rewrite_config(config, conf_id);
    }
    catch (exception e) {
        cout << e.what() << endl;
    }
    return 0;
}
