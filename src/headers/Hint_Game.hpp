<<<<<<< HEAD
#ifndef HINT_GAME_HPP
#define HINT_GAME_HPP

#include "Word_Manager.hpp"
#include <vector>
#include <string>

class HintGame {
protected:
    int hintsAvailable;                     // Number of hints available
    WordManager* wordManager;               // Reference to WordManager
    std::vector<char> guessedLetters;       // Letters that have already been guessed

public:
    explicit HintGame(int maxHints, WordManager* manager = nullptr); // Constructor
    virtual ~HintGame();                    // Destructor

    void setWordManager(WordManager* manager); // Set the WordManager reference
    char getRandomUnrevealedLetter();         // Select a random unrevealed letter
    bool offerHint();                         // Offers a hint if the user accepts
    bool offerHintDirect();                   // Offers a hint directly
    bool requestHintOnIncorrectGuess();       // Request a hint after an incorrect guess
    int getHintsRemaining() const;            // Get remaining hints
};

#endif // HINT_GAME_HPP
=======
//
// Created by Alex on 20.11.2024.
//

#ifndef HINT_GAME_HPP
#define HINT_GAME_HPP
#include "Game.hpp"


class HintGame {
protected:
    int hintsAvailable;            // Numărul de hinturi disponibile
    std::vector<std::string> hints; // Lista de hinturi
public:
    explicit HintGame(int maxHints = 3); // Constructor care setează numărul maxim de hinturi
    virtual ~HintGame() = default;
    void setHints(const std::vector<std::string>& hintList); // Setează hinturile
    void provideHint(); // Oferă un hint
    bool requestHintOnIncorrectGuess(); // Oferă hint dacă ghicitul a fost greșit
    int getHintsRemaining() const; // Returnează câte hinturi mai sunt disponibile
};



#endif //HINT_GAME_HPP
>>>>>>> a1039bfa419783e09da6ceef0121c2b9e26aabc5
