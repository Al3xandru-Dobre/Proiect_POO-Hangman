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
