//
// Created by Alex on 20.11.2024.
//

#include <array>
#include <stdexcept>

#include "headers/Word_Manager.hpp"

std::string DefaultWordManager::getCategoryKey() const { return "mixed"; }
std::string DefaultWordManager::getDisplayName() const { return "Mixt"; }
std::string DefaultWordManager::getDataFile() const { return {}; }

void DefaultWordManager::loadWords() {
    // Combinam toate categoriile. Daca un fisier lipseste, il sarim.
    const std::array<std::string, 3> files = {
        "data/animal_words.txt", "data/technology_words.txt", "data/geography_words.txt"};

    for (const auto& file : files) {
        try {
            loadWordsFromFile(file, getCategoryKey());
        } catch (const std::runtime_error&) {
            // fisier indisponibil: continuam cu celelalte
        }
    }

    // Rezerva: jocul porneste chiar si fara niciun fisier de date.
    if (wordMap[getCategoryKey()].empty()) {
        wordMap[getCategoryKey()] = {"hangman", "object", "class", "program", "keyboard"};
    }
}
