//
// Created by Alex on 20.11.2024.
//

#include "headers/Word_Manager.hpp"

TechnologyWordManager::TechnologyWordManager() {
    loadWordsFromFile("technology_words.txt", "technology");
}

void TechnologyWordManager::selectRandomWordFromCategory() {
    const auto& words = wordMap["technology"];
    if (!words.empty()) {
        selectedWord = words[std::rand() % words.size()];
    } else {
        throw std::runtime_error("No words available in the category: technology");
    }
}