#include <random>
using namespace std;


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
};
