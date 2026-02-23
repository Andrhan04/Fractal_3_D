#include <random>
#include <algorithm>
#include <cmath>

class Generator {
private:
    std::mt19937 random_generator_;  // Генератор случайных чисел на базе Mersenne Twister

    // Вспомогательная функция для получения случайного целого в диапазоне [min, max]
    int returnRandom(int min, int max) {
        if (max < min) std::swap(max, min);  // Обеспечиваем корректный порядок границ
        std::uniform_int_distribution<int> range(min, max);
        return range(random_generator_);
    }

    // Проверка, попадает ли точка (x, y) в круг радиуса sz
    bool chek(double x, double y, double sz) {
        return x * x + y * y <= sz * sz;
    }

    // Вычисляет sz^2 - x^2 (возможно, для последующего извлечения корня)
    double get(double x, double sz) {
        return sz * sz - x * x;
    }

public:
    // Конструктор: инициализируем генератор случайным зерном
    Generator() {
        std::random_device device;
        random_generator_.seed(device());
    }

    // Публичная обёртка для получения случайного целого в диапазоне
    int get_int(int mn, int mx) {
        return returnRandom(std::min(mn, mx), std::max(mx, mn));
    }
    
	// Публичная функция для получения случайного вещественного числа в диапазоне [mn, mx]
    double get_double(double mn, double mx) {
        if (mx < mn) std::swap(mx, mn);  // Обеспечиваем корректный порядок границ
        std::uniform_real_distribution<double> range(mn, mx);
        return range(random_generator_);
	}
};
