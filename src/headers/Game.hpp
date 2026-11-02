//
// Created by Alex on 20.11.2024.
//

#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "Game_Timer.hpp"

class HintGame;
class WordManager;

// Setarile unei runde. Fiecare nivel are o fabrica statica.
struct Difficulty {
    std::string name;
    int maxAttempts;      // cate greseli sunt permise (6 = cap, corp, 2 maini, 2 picioare)
    int hints;            // cate hint-uri primeste jucatorul
    int timeLimitSeconds; // 0 = fara limita
    int scoreMultiplier;

    static Difficulty easy();
    static Difficulty normal();
    static Difficulty hard();
};

// Rezultatul unei incercari; interfata (consola sau grafica) decide cum il afiseaza.
enum class GuessResult {
    Correct,        // litera/cuvant corect
    Wrong,          // gresit -> se pierde o incercare
    AlreadyGuessed, // litera deja incercata -> nu se penalizeaza
    Invalid,        // caracter care nu e litera -> nu se penalizeaza
    GameFinished    // runda s-a terminat deja
};

// Logica unei runde. Game NU afiseaza nimic: doar tine starea si aplica regulile.
// Astfel aceeasi clasa este folosita si de ConsoleUI, si de interfata SFML.
class Game {
protected:
    // Ordinea declararii = ordinea initializarii (vezi lista din constructor).
    std::unique_ptr<WordManager> wordManager; // compozitie: Game detine categoria
    std::unique_ptr<HintGame> hintGame;       // compozitie: Game detine hint-urile
    Difficulty difficulty;
    GameTimer timer;
    std::string guessedWord;       // progresul, ex. "m_unt__n"
    std::set<char> triedLetters;   // toate literele incercate
    std::vector<char> wrongLetters;// literele gresite, in ordinea incercarii
    int attemptsRemaining;
    bool timeExpired = false;

    int revealLetter(char letter);  // intoarce cate pozitii au fost dezvaluite
    void finishIfOver();

public:
    Game(std::unique_ptr<WordManager> manager, std::unique_ptr<HintGame> hintManager,
         Difficulty settings = Difficulty::normal());

    // Game detine resurse unice -> nu poate fi copiat, doar mutat.
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) noexcept;
    Game& operator=(Game&&) noexcept;
    virtual ~Game(); // definit in Game.cpp, unde HintGame/WordManager sunt tipuri complete

    GuessResult guessLetter(char letter);
    GuessResult guessWord(const std::string& word);
    std::optional<char> useHint(); // dezvaluie o litera, nu consuma incercari
    void update();                 // verifica expirarea timpului

    [[nodiscard]] bool isGameWon() const;
    [[nodiscard]] bool isGameLost() const;
    [[nodiscard]] bool isGameOver() const; // castigat SAU pierdut
    [[nodiscard]] bool isTimeUp() const;

    [[nodiscard]] int getAttemptsRemaining() const;
    [[nodiscard]] int getMaxAttempts() const;
    [[nodiscard]] int getMistakes() const;
    [[nodiscard]] const std::string& getGuessedWord() const;
    [[nodiscard]] const std::string& getSelectedWord() const;
    [[nodiscard]] const std::vector<char>& getWrongLetters() const;
    [[nodiscard]] bool wasLetterTried(char letter) const;
    [[nodiscard]] const HintGame& getHintGame() const;
    [[nodiscard]] const WordManager& getWordManager() const;
    [[nodiscard]] const GameTimer& getTimer() const;
    [[nodiscard]] const Difficulty& getDifficulty() const;

    [[nodiscard]] int computeScore() const;
};

#endif //GAME_HPP
