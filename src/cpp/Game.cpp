//
// Created by Alex on 20.11.2024.
//

#include "headers/Game.hpp"
#include "headers/Hint_Game.hpp"
#include "headers/Word_Manager.hpp"
#include <iostream>
#include <vector>
#include <memory>

Game::Game(std::unique_ptr<HintGame>hint_manager ,std::unique_ptr<WordManager> manager, int attempts)
      : hintGame(std::move(hint_manager)),
        wordManager(std::move(manager)),
        attemptsRemaining(attempts) {
    wordManager->selectRandomWordFromCategory();
    guessedWord = std::string(wordManager->getSelectedWord().size(), '_');
}

bool Game::guessLetter(char letter) {
    bool found = false;
    for (size_t i = 0; i < wordManager->getSelectedWord().size(); i++) {
        if (wordManager->getSelectedWord()[i] == letter) {
            guessedWord[i] = letter;
            found = true;
        }
    }
    if (!found) {
        attemptsRemaining--;
        handleHintRequest();
    }
    return found;
}

bool Game::guessWord(const std::string& word) {
    if (word == wordManager->getSelectedWord()) {
        guessedWord = word;
        return true;
    } else {
        attemptsRemaining--;
        handleHintRequest();
        return false;
    }
}

bool Game::isGameWon() const {
    return guessedWord == wordManager->getSelectedWord();
}

bool Game::isGameOver() const {
    return attemptsRemaining <= 0;
}

    int Game::getAttemptsRemaining() const {
        return attemptsRemaining;
    }

    std::string Game::getGuessedWord() const {
        return guessedWord;
    }

std::string Game::getSelectedWord() const {
    return wordManager->getSelectedWord();
}

void Game::displayStatus() const {
    std::cout << "Cuvant: " << guessedWord << "\n";
    std::cout << "Incercari ramase: " << attemptsRemaining << "\n";
}

void Game::handleHintRequest() {
    if (hintGame->getHintsRemaining() > 0) {
        hintGame->requestHintOnIncorrectGuess(); // Oferă hint dacă este disponibil
    }
}