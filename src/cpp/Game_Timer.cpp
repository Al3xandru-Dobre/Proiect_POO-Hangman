//
// Created by Alex on 20.11.2024.
//

#include "headers/Game_Timer.hpp"

#include <algorithm>
#include <cmath>

GameTimer::GameTimer(int limitSeconds)
    : startTime(Clock::now()), stopTime(startTime), timeLimit(std::max(0, limitSeconds)) {}

GameTimer::Clock::time_point GameTimer::now() const {
    return running ? Clock::now() : stopTime;
}

void GameTimer::restart() {
    startTime = Clock::now();
    running = true;
}

void GameTimer::stop() {
    if (running) {
        stopTime = Clock::now();
        running = false;
    }
}

bool GameTimer::hasLimit() const {
    return timeLimit > 0;
}

int GameTimer::getTimeLimit() const {
    return timeLimit;
}

float GameTimer::getElapsedSeconds() const {
    return std::chrono::duration<float>(now() - startTime).count();
}

bool GameTimer::isTimeUp() const {
    return hasLimit() && getElapsedSeconds() >= static_cast<float>(timeLimit);
}

int GameTimer::getTimeRemaining() const {
    if (!hasLimit()) {
        return 0;
    }
    const float remaining = static_cast<float>(timeLimit) - getElapsedSeconds();
    return std::max(0, static_cast<int>(std::ceil(remaining)));
}

float GameTimer::getRemainingFraction() const {
    if (!hasLimit()) {
        return 1.0f;
    }
    return std::clamp(1.0f - getElapsedSeconds() / static_cast<float>(timeLimit), 0.0f, 1.0f);
}
