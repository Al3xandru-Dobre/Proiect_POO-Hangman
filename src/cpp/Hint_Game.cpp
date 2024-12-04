#include "src/headers/Hint_Game.hpp"
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
            std::cout << "Hint: Litera '" << hint << "' se afla in cuvant!\n";
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
                std::cout << "Hint: Litera '" << hint << "' se afla in cuvant!\n";
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
    }
    return false;
}

// Request a hint after an incorrect guess
bool HintGame::requestHintOnIncorrectGuess() {
    return offerHint();
}

// Get the number of hints remaining
int HintGame::getHintsRemaining() const {
    return hintsAvailable;
}
