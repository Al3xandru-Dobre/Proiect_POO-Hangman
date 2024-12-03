//
// Created by Alex on 20.11.2024.
//

#include "headers/Word_Manager.hpp"


GeographyWordManager::GeographyWordManager() {
    loadWordsFromFile("geography_words.txt", "geography");
}

void GeographyWordManager::selectRandomWordFromCategory() {
    const auto& words = wordMap["geography"];
    if (!words.empty()) {
        selectedWord = words[std::rand() % words.size()];
    } else {
        throw std::runtime_error("No words available in the category: geography");
    }
}
