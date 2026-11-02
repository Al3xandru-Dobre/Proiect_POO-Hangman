//
// Created by Alex on 20.11.2024.
//

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

// Generator aleatoriu unic pentru tot programul.
// Inlocuieste std::srand/std::rand: un singur motor Mersenne Twister,
// initializat o singura data din std::random_device, plus distributii
// uniforme (fara biasul lui rand() % n).
class Random {
public:
    Random() = delete; // clasa utilitara: doar metode statice

    static std::mt19937& engine() {
        static std::mt19937 generator{std::random_device{}()};
        return generator;
    }

    static int between(int min, int max) {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(engine());
    }

    static float realBetween(float min, float max) {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(engine());
    }
};

#endif //RANDOM_HPP
