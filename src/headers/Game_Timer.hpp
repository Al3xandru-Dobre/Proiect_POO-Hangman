//
// Created by Alex on 20.11.2024.
//

#ifndef GAME_TIMER_HPP
#define GAME_TIMER_HPP

#include <chrono>

// Cronometru de runda bazat pe std::chrono::steady_clock (monoton, nu sare
// daca se schimba ora sistemului). O limita de 0 secunde inseamna "fara limita".
class GameTimer {
private:
    using Clock = std::chrono::steady_clock;

    Clock::time_point startTime;
    Clock::time_point stopTime;
    bool running = true;
    int timeLimit; // secunde

    [[nodiscard]] Clock::time_point now() const;

public:
    explicit GameTimer(int limitSeconds = 0);

    void restart();
    void stop(); // ingheata timpul (la finalul rundei)

    [[nodiscard]] bool hasLimit() const;
    [[nodiscard]] int getTimeLimit() const;
    [[nodiscard]] float getElapsedSeconds() const;
    [[nodiscard]] bool isTimeUp() const;
    [[nodiscard]] int getTimeRemaining() const;      // niciodata negativ
    [[nodiscard]] float getRemainingFraction() const; // 1.0 -> 0.0 (pentru bara de timp)
};

#endif //GAME_TIMER_HPP
