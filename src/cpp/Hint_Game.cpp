<<<<<<< HEAD
#include "headers/Hint_Game.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <random>

// Constructor
HintGame::HintGame(int maxHints, WordManager* manager)
    : hintsAvailable(maxHints), wordManager(manager) {}

// Destructor
HintGame::~HintGame() = default;

// Set the WordManager
void HintGame::setWordManager(WordManager* manager) {
    wordManager = manager;
}

// Return a random unrevealed letter from the WordManager's selected word
char HintGame::getRandomUnrevealedLetter() {
    if (!wordManager) {
        throw std::runtime_error("WordManager is not set!");
    }

    const std::string& word = wordManager->getSelectedWord();
    std::vector<char> unrevealedLetters;

    // Collect letters from the word that have not been guessed yet
    for (char letter : word) {
        if (std::find(guessedLetters.begin(), guessedLetters.end(), letter) == guessedLetters.end()) {
            unrevealedLetters.push_back(letter);
        }
    }

    if (unrevealedLetters.empty()) {
        throw std::runtime_error("All letters are revealed!");
    }

    // Select a random letter
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, unrevealedLetters.size() - 1);

    return unrevealedLetters[distr(gen)];
}
bool HintGame::offerHintDirect() {
    if (hintsAvailable > 0) {
        try {
            char hint = getRandomUnrevealedLetter();
            std::cout << "Hint: Litera '" << hint << "' se afla în cuvânt!\n";
            hintsAvailable--;
            return true;
        } catch(const std::runtime_error& e) {
            std::cout << "Nu pot oferi un hint: " << e.what() << "\n";
        }
        return false;
    }
}


// Offers a hint to the user
bool HintGame::offerHint() {
    if (hintsAvailable > 0) {
        std::string choice;
        std::cout << "Doresti un hint? (Da/Nu): ";
        std::cin >> choice;

        if (choice == "Da" || choice == "da") {
            try {
                char hint = getRandomUnrevealedLetter();
                std::cout << "Hint: Litera '" << hint << "' se află în cuvânt!\n";
                hintsAvailable--;
                return true;
            } catch (const std::runtime_error& e) {
                std::cout << "Nu pot oferi un hint: " << e.what() << "\n";
            }
        } else {
            std::cout << "Nu s-a oferit niciun hint.\n";
        }
    } else {
        std::cout << "Nu mai sunt hinturi disponibile.\n";
=======
//
// Created by Alex on 20.11.2024.
//

#include "headers/Hint_Game.hpp"
#include <iostream>
HintGame::HintGame(int maxHints) : hintsAvailable(maxHints) {}

void HintGame::setHints(const std::vector<std::string>& hintList) {
    hints = hintList;
}

void HintGame::provideHint() {
    if (hintsAvailable > 0 && !hints.empty()) {
        std::cout << "Hint: " << hints.back() << "\n"; // Afișează hint-ul
        hints.pop_back(); // Îndepărtează hint-ul folosit
        hintsAvailable--; // Scade numărul de hinturi disponibile
    } else {
        std::cout << "Nu mai sunt hint-uri disponibile.\n";
    }
}

bool HintGame::requestHintOnIncorrectGuess() {
    if (hintsAvailable > 0) {
        provideHint();
        return true;
>>>>>>> a1039bfa419783e09da6ceef0121c2b9e26aabc5
    }
    return false;
}

<<<<<<< HEAD
// Request a hint after an incorrect guess
bool HintGame::requestHintOnIncorrectGuess() {
    return offerHint();
}

// Get the number of hints remaining
int HintGame::getHintsRemaining() const {
    return hintsAvailable;
}
=======
int HintGame::getHintsRemaining() const {
    return hintsAvailable;
}
>>>>>>> a1039bfa419783e09da6ceef0121c2b9e26aabc5
