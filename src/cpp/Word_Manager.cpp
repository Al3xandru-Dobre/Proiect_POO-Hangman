//
// Created by Alex on 20.11.2024.
//

#include "src/headers/Word_Manager.hpp"
#include <ctime>
#include <cstdlib>
#include <fstream>


WordManager::WordManager() {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Inițializare RNG
}
void WordManager::loadWordsFromFile(const std::string& filePath, const std::string& category) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::string word;
    while (std::getline(file, word)) {
        if (!word.empty()) {
            wordMap[category].push_back(word);
        }
    }
    file.close();
}

std::string WordManager::getSelectedWord() const {
        return selectedWord;
    }

bool WordManager::isLetterInWord(char letter) const {
        return selectedWord.find(letter) != std::string::npos;
    }


