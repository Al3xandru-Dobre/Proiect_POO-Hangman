//
// Created by Alex on 20.11.2024.
//

#include "Animal.hpp"

AnimalWordManager::AnimalWordManager() {
    loadWordsFromFile("animal_words.txt", "animals");
}

void AnimalWordManager::selectRandomWordFromCategory() {
    const auto& words = wordMap["animals"];
    if (!words.empty()) {
        selectedWord = words[std::rand() % words.size()];
    } else {
        throw std::runtime_error("No words available in the category: animals");
    }
}
