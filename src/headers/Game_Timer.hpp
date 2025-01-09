//
// Created by Alex on 20.11.2024.
//

#ifndef GAME_TIMER_HPP
#define GAME_TIMER_HPP
#include <chrono>
#include <iostream>
#include <cstdlib>

/*
 *Aceasta clasa are ca scop gestionarea timpului de joc
 *Nu am setat pentru modul de joc, dar, in cazul in care se doreste, se poate seta un timp limita pentru joc
 *In momentul acesta serveste ca flag pentru inchiderea jocului
 */

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
