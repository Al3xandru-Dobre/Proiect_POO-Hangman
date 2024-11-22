//
// Created by Alex on 20.11.2024.
//

#include "headers/Hint_Game.hpp"
#include <iostream>
HintGame::HintGame(int maxHints) : hintsAvailable(maxHints) {}

void HintGame::setHints(const std::vector<std::string>& hintList) {
    hints = hintList;
}

void HintGame::provideHint() {
    if (hintsAvailable > 0 && !hints.empty()) {
        std::cout << "Hint: " << hints.back() << "\n"; // Afișează hint-ul
        hints.pop_back(); // Îndepărtează hint-ul folosit
        hintsAvailable--; // Scade numărul de hinturi disponibile
    } else {
        std::cout << "Nu mai sunt hint-uri disponibile.\n";
    }
}

bool HintGame::requestHintOnIncorrectGuess() {
    if (hintsAvailable > 0) {
        provideHint();
        return true;
    }
    return false;
}

int HintGame::getHintsRemaining() const {
    return hintsAvailable;
}