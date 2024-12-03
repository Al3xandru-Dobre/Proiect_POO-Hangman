//
// Created by Alex on 20.11.2024.
//

#ifndef HANGMAN_UI_HPP
#define HANGMAN_UI_HPP
#include "Hint_Game.hpp"
#include "Game.hpp"
class HangmanUI {
protected:
<<<<<<< HEAD
    Game& game;
    HintGame hintGame;
public:
    HangmanUI(Game& gameInstance, HintGame* hintGameInstance);
    void start();
    [[nodiscard]] bool gameWon() const;
=======
    Game Game;
public:
    HangmanUI();
    void start(HintGame game);
    bool gameWon() const;
>>>>>>> a1039bfa419783e09da6ceef0121c2b9e26aabc5
};



#endif //HANGMAN_UI_HPP
