//
// Created by Alex on 20.11.2024.
//

#ifndef GAME_TIMER_HPP
#define GAME_TIMER_HPP
#include <chrono>
#include <iostream>
#include <cstdlib>

class GameTimer {
private:
    std::chrono::steady_clock::time_point startTime;
    int timeLimit;
public:
    GameTimer(int limit);

    [[nodiscard]] bool isTimeUp() const;

    [[nodiscard]] int getTimeRemaining() const ;

    virtual ~GameTimer() = default;
};



#endif //GAME_TIMER_HPP
