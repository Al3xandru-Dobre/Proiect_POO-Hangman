//
// Created by Alex on 20.11.2024.
//

#ifndef HANGMAN_UI_HPP
#define HANGMAN_UI_HPP
#include "Hint_Game.hpp"
#include "Game.hpp"
class HangmanUI {
protected:
    Game Game;
public:
    HangmanUI();
    void start(HintGame game);
    bool gameWon() const;
};



#endif //HANGMAN_UI_HPP
