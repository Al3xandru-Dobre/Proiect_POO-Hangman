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
<<<<<<< HEAD

    virtual ~GameTimer() = default;
=======
>>>>>>> a1039bfa419783e09da6ceef0121c2b9e26aabc5
};



#endif //GAME_TIMER_HPP
