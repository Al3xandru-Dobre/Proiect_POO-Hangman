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

/*
 *Aceasta clasa are ca scop gestionarea cuvintelor
 *Cuvintele sunt alese random dintr-un fisier text, in functie de categoria aleasa
 *Am folosit un hash table pentru a stoca cuvintele, pentru a le putea accesa rapid
 *are mai multe derivate pentru categorii diferite de cuvinte
 */
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

#endif //WORD_MANAGER_HPP
