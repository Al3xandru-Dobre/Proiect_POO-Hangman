//
// Created by Alex on 20.11.2024.
//

#ifndef HANGMAN_UI_HPP
#define HANGMAN_UI_HPP
#include "Hint_Game.hpp"
#include "Game.hpp"
class HangmanUI {
protected:
    Game& game;
    HintGame hintGame;
public:
    HangmanUI(Game& gameInstance, HintGame* hintGameInstance);
    void start();
    [[nodiscard]] bool gameWon() const;

    Game* operator->();
    const Game* operator->() const;

    HangmanUI(const HangmanUI& obj) = delete;
    HangmanUI& operator=(const HangmanUI& obj) = delete;
    HangmanUI(HangmanUI&& obj) noexcept;
    HangmanUI& operator=(HangmanUI&& obj) noexcept ;
};



#endif //HANGMAN_UI_HPP
