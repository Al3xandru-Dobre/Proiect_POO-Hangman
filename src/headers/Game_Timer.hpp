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

    bool isTimeUp() const;

    int getTimeRemaining() const ;

    virtual ~GameTimer() = default;
};



#endif //GAME_TIMER_HPP
