//
// Created by Alex on 20.11.2024.
//

#ifndef GAME_HPP
#define GAME_HPP
#include <memory>
#include <string>
#include <vector>
#include "Scor.hpp"

class HintGame;
class WordManager;
class GameTimer;


class Game {
protected:
    std::unique_ptr<WordManager> wordManager; // Pointer pentru gestiunea dinamică a categoriei
    std::string guessedWord; // Progresul cuvântului ghicit
    int attemptsRemaining;
    std::unique_ptr<HintGame> hintGame; // Pointer pentru gestiunea hinturilor
    std::unique_ptr<Scor> scor; //Pointer pentru a gestiona scorul jucatorului
public:
    explicit Game(std::unique_ptr<HintGame> hint_manager,std::unique_ptr<WordManager> manager,std::unique_ptr<Scor> scor ,int attempts = 6);
    Game() = default;
    bool guessLetter(char letter); // Ghicirea unei litere
    bool guessWord(const std::string& word); // Ghicirea unui cuvânt
    [[nondiscard]] bool isGameWon() const; // Verifică dacă jocul a fost câștigat
    [[nondiscard]] bool isGameOver() const; // Verifică dacă jocul s-a terminat
    [[nodiscard]] int getAttemptsRemaining() const; // Returnează numărul de încercări rămase
    [[nodiscard]] std::string getGuessedWord() const; // Returnează progresul cuvântului ghicit
    [[nodiscard]] std::string getSelectedWord() const; // Returnează cuvântul selectat
    [[nodiscard]] HintGame* getHintGame() const; // Returnează pointer la obiectul HintGame
    [[nondiscard]] void displayStatus() const; // Afișează starea jocului
    void handleHintRequest(); // Metodă care gestionează cererea de hint
    void displayScore() const; // Afișează scorul curent

    Game(Game && obj) noexcept;
    Game& operator=(Game && obj) noexcept;

    Game(const Game& obj) = delete;
    Game& operator=(const Game& obj) = delete;


    Scor* operator->();
    const Scor* operator->() const;

    virtual ~Game() = default;
};



#endif //GAME_HPP
