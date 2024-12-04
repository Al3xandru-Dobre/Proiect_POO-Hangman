//
// Created by Alex on 20.11.2024.
//

#include "src/headers/Game_Timer.hpp"



    GameTimer::GameTimer(int limit) : timeLimit(limit) {
        startTime = std::chrono::steady_clock::now();
    }

    bool GameTimer::isTimeUp() const {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        return elapsed >= timeLimit;
    }

    int GameTimer::getTimeRemaining() const {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        return timeLimit - elapsed;
    }

