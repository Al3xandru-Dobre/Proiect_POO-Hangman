//
// Created by Alex on 03.12.2024.
//

#include "Word_Manager.hpp"



void DefaultWordManager::selectRandomWordFromCategory() {
    std::cout << "Selectam cuvinte din categoria default" << std::endl;
    loadWordsFromFile("defaults.txt", "defaults");
    const auto& words = wordMap["defaults"];
    if (!words.empty()) {
        selectedWord = words[std::rand() % words.size()];
    } else {
        throw std::runtime_error("No words available in the category: defaults");
    }
}
