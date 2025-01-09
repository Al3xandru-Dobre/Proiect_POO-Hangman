//
// Created by Alex on 20.11.2024.
//

#ifndef HANGMAN_UI_HPP
#define HANGMAN_UI_HPP
#include "Hint_Game.hpp"
#include "Game.hpp"
#include "Observer.hpp"
/*
 *Aceasta clasa are ca scop gestionarea interfetei jocului
 *Ea are ca rol sa afiseze starea jocului, sa gestioneze inputul jucatorului si sa afiseze scorul
 *pe langa asta, aceasta clasa este si un observer pentru joc, astfel incat sa poata fi notificata de schimbarile din joc
 *Hinturile sunt gestionate de o clasa separata, care este pasata ca parametru in constructor,astfel incat pot avea acces versatil la Hints
 */
class HangmanUI : public Observer{
protected:
    Game& game;
    HintGame hintGame;
public:
    HangmanUI(Game& gameInstance, HintGame* hintGameInstance);
    void start();
    [[nodiscard]] bool gameWon() const;
    ~HangmanUI() override {
        game.removeObserver(this);
    }
    void update() override {
        std::cout <<"Starea jocului s-a schimbat\n";
    }

    Game* operator->();
    const Game* operator->() const;

    HangmanUI(const HangmanUI& obj) = delete;
    HangmanUI& operator=(const HangmanUI& obj) = delete;
    HangmanUI(HangmanUI&& obj) noexcept;
    HangmanUI& operator=(HangmanUI&& obj) noexcept ;
};



#endif //HANGMAN_UI_HPP
