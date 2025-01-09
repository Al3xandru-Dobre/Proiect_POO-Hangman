//
// Created by Alex on 20.11.2024.
//

#ifndef GAME_HPP
#define GAME_HPP
#include <memory>
#include <string>
#include <vector>
#include "Scor.hpp"
#include "Subject.hpp"
#include "Observer.hpp"
#include "Subject.hpp"
#include <list>
class HintGame;
class WordManager;
class GameTimer;
//WIP PENTRU IMPLEMENTAREA CA SUBIECT --- DONE

/*Aceasta clasa are ca scop gestionarea parcursului jocului
 *Logica de introducere a literei sau cuvantului, a verificarii daca jocul a fost castigat sau pierdut
 *Acordul de puncte
 *Totodata, aceasta clasa contine pointeri pentru a gestiona hinturile, cuvintele si scorul catre alte clase ce sunt responsabile de acestea
 *Metodele elaborate se folosesc si de functionalitatile celorlalte clase pentru a oferi o experienta de joc cat mai placuta
 *
 *
 *GAME este subiectul pentru Observer Pattern
 *am implementat acest Pattern pe un template de clasa Subject care primeste ca parametru un tip de obiect Observer
 *implementarea se face in functii de adaugare, stergere si notificare a observerilor
 *am ales sa fac asta pentru a putea genera mai multe tipuri de observatoare pentru joc,cat si pentru scor
 *implementarea de observeri se face in clasa Observer, care este legata de HanmanUI pentru a fi notificat de evolutia jocului si de adaptarea interfetei
 */

class Game :public Subject<Observer> {
protected:
    std::list<Observer*> list_observer;
    std::string message;
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

    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void notifyObservers();

    Game(Game && obj) noexcept;
    Game& operator=(Game && obj) noexcept;

    Game(const Game& obj) = delete;
    Game& operator=(const Game& obj) = delete;

    Scor* operator->();
    const Scor* operator->() const;

    virtual ~Game() = default;
};



#endif //GAME_HPP
