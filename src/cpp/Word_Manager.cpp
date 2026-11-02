//
// Created by Alex on 20.11.2024.
//

#include "headers/Word_Manager.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>

#include "headers/Random.hpp"
#include "headers/Resources.hpp"

std::string WordManager::normalizeWord(const std::string& rawLine) {
    // 1. Eliminam spatiile de la capete, inclusiv '\r' din fisierele Windows (CRLF).
    const auto isSpace = [](unsigned char c) { return std::isspace(c) != 0; };
    auto first = std::find_if_not(rawLine.begin(), rawLine.end(), isSpace);
    auto last = std::find_if_not(rawLine.rbegin(), rawLine.rend(), isSpace).base();
    if (first >= last || *first == '#') {
        return {}; // linie goala sau comentariu
    }

    // 2. Litere mici; acceptam doar litere, spatiu si cratima.
    std::string word;
    bool hasLetter = false;
    for (auto it = first; it != last; ++it) {
        const auto c = static_cast<unsigned char>(*it);
        if (std::isalpha(c)) {
            word += static_cast<char>(std::tolower(c));
            hasLetter = true;
        } else if (c == ' ' || c == '-') {
            word += static_cast<char>(c);
        } else {
            return {}; // caracter neacceptat (cifre, diacritice UTF-8 etc.)
        }
    }
    return hasLetter ? word : std::string{};
}

void WordManager::loadWordsFromFile(const std::string& fileName, const std::string& category) {
    std::ifstream file(resolveResourcePath(fileName));
    if (!file.is_open()) {
        throw std::runtime_error("Nu pot deschide fisierul: " + fileName);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string word = normalizeWord(line);
        if (!word.empty()) {
            wordMap[category].push_back(std::move(word));
        }
    }
}

void WordManager::loadWords() {
    loadWordsFromFile(getDataFile(), getCategoryKey());
}

void WordManager::selectRandomWordFromCategory() {
    // Incarcarea este "lenesa": nu o facem in constructor, pentru ca acolo
    // apelurile virtuale nu ajung inca la clasa derivata.
    const std::string key = getCategoryKey();
    if (wordMap[key].empty()) {
        loadWords();
    }

    const auto& words = wordMap[key];
    if (words.empty()) {
        throw std::runtime_error("Nu exista cuvinte in categoria: " + getDisplayName());
    }
    selectedWord = words[static_cast<std::size_t>(Random::between(0, static_cast<int>(words.size()) - 1))];
}

const std::string& WordManager::getSelectedWord() const {
    return selectedWord;
}

bool WordManager::isLetterInWord(char letter) const {
    return selectedWord.find(letter) != std::string::npos;
}

std::size_t WordManager::getWordCount() const {
    const auto it = wordMap.find(getCategoryKey());
    return it == wordMap.end() ? 0 : it->second.size();
}

std::unique_ptr<WordManager> createWordManager(Category category) {
    switch (category) {
        case Category::Animals:    return std::make_unique<AnimalWordManager>();
        case Category::Technology: return std::make_unique<TechnologyWordManager>();
        case Category::Geography:  return std::make_unique<GeographyWordManager>();
        case Category::Mixed:      break;
    }
    return std::make_unique<DefaultWordManager>();
}
