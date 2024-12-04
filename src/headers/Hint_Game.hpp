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
    [[nodiscard]] int getHintsRemaining() const;            // Get remaining hints
};

#endif // HINT_GAME_HPP
