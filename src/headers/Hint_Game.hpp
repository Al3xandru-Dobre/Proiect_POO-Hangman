#ifndef HINT_GAME_HPP
#define HINT_GAME_HPP

#include "Word_Manager.hpp"
#include <vector>
#include <string>
/*
 *Aceasta clasa are ca scop gestionarea hinturilor
 *folosesete un pointer catre WordManager pentru a putea accesa cuvintele si a oferi hinturi, cu litere unice alese din cuvant
 *poate sa ofere hinturi directe sau sa astepte cererea jucatorului
 *da reveal la toate pozitile cu litera aleasa
 *foloseste o functie din C++11 pentru a alege un numar random
 */

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

    HintGame(const HintGame& obj) ;
    HintGame& operator=(const HintGame& obj) ;

    //HintGame(HintGame&& obj) noexcept;
    //HintGame& operator=(HintGame&& obj) noexcept;
};

#endif // HINT_GAME_HPP
