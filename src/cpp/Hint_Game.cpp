//
// Created by Alex on 20.11.2024.
//

#include "headers/Hint_Game.hpp"

#include <algorithm>
#include <vector>

#include "headers/Random.hpp"

HintGame::HintGame(int maxHints) : hintsAvailable(std::max(0, maxHints)) {}

std::optional<char> HintGame::getRandomUnrevealedLetter(const std::string& secret,
                                                        const std::string& progress) const {
    std::vector<char> candidates;
    const std::size_t length = std::min(secret.size(), progress.size());
    for (std::size_t i = 0; i < length; ++i) {
        if (progress[i] == '_' &&
            std::find(candidates.begin(), candidates.end(), secret[i]) == candidates.end()) {
            candidates.push_back(secret[i]);
        }
    }
    if (candidates.empty()) {
        return std::nullopt;
    }
    return candidates[static_cast<std::size_t>(Random::between(0, static_cast<int>(candidates.size()) - 1))];
}

std::optional<char> HintGame::provideHint(const std::string& secret, const std::string& progress) {
    if (!canProvideHint()) {
        return std::nullopt;
    }
    const auto letter = getRandomUnrevealedLetter(secret, progress);
    if (letter) { // consumam hint-ul doar daca a dezvaluit ceva
        --hintsAvailable;
        ++hintsUsed;
    }
    return letter;
}

bool HintGame::canProvideHint() const {
    return hintsAvailable > 0;
}

int HintGame::getHintsRemaining() const {
    return hintsAvailable;
}

int HintGame::getHintsUsed() const {
    return hintsUsed;
}
