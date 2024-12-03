//
// Created by Alex on 20.11.2024.
//

#ifndef WORD_MANAGER_HPP
#define WORD_MANAGER_HPP
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

class WordManager {
protected:
    std::unordered_map<std::string, std::vector<std::string>> wordMap; // Hash table pentru categorii de cuvinte
    std::string selectedWord;
    void loadWordsFromFile(const std::string& filePath, const std::string& category);

public:
    WordManager();

    virtual ~WordManager() = default;

    virtual void selectRandomWordFromCategory() = 0;

    [[nodiscard]] std::string getSelectedWord() const;

    [[nodiscard]] bool isLetterInWord(char letter) const;
};

class AnimalWordManager : public WordManager {
public:
    AnimalWordManager();

    void selectRandomWordFromCategory() override;
};


class TechnologyWordManager : public WordManager {
public:
    TechnologyWordManager();

    void selectRandomWordFromCategory() override;
};


class DefaultWordManager : public WordManager {
public:
    DefaultWordManager() = default;
    void selectRandomWordFromCategory() override;
};


class GeographyWordManager : public WordManager {
public:
    GeographyWordManager();

    void selectRandomWordFromCategory() override;
};

#endif //WORD_MANAGER_HPP
