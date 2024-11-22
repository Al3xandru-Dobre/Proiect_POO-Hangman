//
// Created by Alex on 20.11.2024.
//

#ifndef GAME_HPP
#define GAME_HPP
#include <memory>
#include <string>
#include <vector>

class HintGame;
class WordManager;

class Game {
protected:
    std::unique_ptr<WordManager> wordManager; // Pointer pentru gestiunea dinamică a categoriei
    std::string guessedWord; // Progresul cuvântului ghicit
    int attemptsRemaining;
    std::unique_ptr<HintGame> hintGame; // Pointer pentru gestiunea hinturilor


public:
    explicit Game(std::unique_ptr<HintGame> hint_manager,std::unique_ptr<WordManager> manager, int attempts = 6);
    bool guessLetter(char letter); // Ghicirea unei litere
    bool guessWord(const std::string& word); // Ghicirea unui cuvânt
    bool isGameWon() const; // Verifică dacă jocul a fost câștigat
    bool isGameOver() const; // Verifică dacă jocul s-a terminat
    int getAttemptsRemaining() const; // Returnează numărul de încercări rămase
    std::string getGuessedWord() const; // Returnează progresul cuvântului ghicit
    std::string getSelectedWord() const; // Returnează cuvântul selectat
    void displayStatus() const; // Afișează starea jocului
    void handleHintRequest(); // Metodă care gestionează cererea de hint
    virtual ~Game() = default;
};



#endif //GAME_HPP
