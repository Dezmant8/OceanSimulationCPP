#ifndef RNG_HPP
#define RNG_HPP
#include <random>

class Rng {
public:
    static Rng& instance() {
        static Rng rng;
        return rng;
    }

    int intInRange(int a, int b) {
        std::uniform_int_distribution<int> d(a, b);
        return d(engine_);
    }

    bool chance(int percent) { return intInRange(0, 99) < percent; }
    std::mt19937& engine() { return engine_; }

private:
    Rng() : engine_(std::random_device{}()) {}
    std::mt19937 engine_;
};
#endif
