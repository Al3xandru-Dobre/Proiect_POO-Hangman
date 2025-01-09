// Updated Game.hpp Implementation

#include "Game.hpp"
#include "Hint_Game.hpp"
#include "Word_Manager.hpp"
#include "Game_Timer.hpp"
#include "Scor.hpp"
#include <iostream>
#include <memory>

Game::Game(std::unique_ptr<HintGame> hint_manager, std::unique_ptr<WordManager> manager, std::unique_ptr<Scor> scor, int attempts)
    : hintGame(std::move(hint_manager)),
      wordManager(std::move(manager)),
      scor(std::move(scor)),
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
            scor->adaugaScor(10);
        }
    }
    if (!found) {
        attemptsRemaining--;
        handleHintRequest();
    }
    notifyObservers();
    return found;
}

bool Game::guessWord(const std::string& word) {
    if (word == wordManager->getSelectedWord()) {
        guessedWord = word;
        scor->adaugaScor(50);
        notifyObservers();
        return true;
    } else {
        attemptsRemaining--;
        scor->adaugaScor(-10);
        handleHintRequest();
        notifyObservers();
        return false;
    }
}

[[nodiscard]] bool Game::isGameWon() const {
    return guessedWord == wordManager->getSelectedWord();
}

[[nodiscard]] bool Game::isGameOver() const {
    return attemptsRemaining <= 0;
}

[[nodiscard]] int Game::getAttemptsRemaining() const {
    return attemptsRemaining;
}

[[nodiscard]] HintGame* Game::getHintGame() const {
    return hintGame.get();
}

[[nodiscard]] std::string Game::getGuessedWord() const {
    return guessedWord;
}

[[nodiscard]] std::string Game::getSelectedWord() const {
    return wordManager->getSelectedWord();
}

[[nodiscard]] void Game::displayStatus() const {
    std::cout << "Cuvant: " << guessedWord << "\n";
    std::cout << "Incercari ramase: " << attemptsRemaining << "\n";
}

void Game::handleHintRequest() {
    if (hintGame->getHintsRemaining() > 0) {
        hintGame->requestHintOnIncorrectGuess();
    }
}

Scor* Game::operator->() {
    return scor.get();
}

const Scor* Game::operator->() const {
    return scor.get();
}

void Game::displayScore() const {
    scor->afiseazaScor();
}

Game::Game(Game&& obj) noexcept
    : wordManager(std::move(obj.wordManager)),
      guessedWord(std::move(obj.guessedWord)),
      attemptsRemaining(obj.attemptsRemaining),
      hintGame(std::move(obj.hintGame)),
      scor(std::move(obj.scor)) {}

Game& Game::operator=(Game&& obj) noexcept {
    if (this != &obj) {
        hintGame = std::move(obj.hintGame);
        wordManager = std::move(obj.wordManager);
        guessedWord = std::move(obj.guessedWord);
        attemptsRemaining = obj.attemptsRemaining;
        scor = std::move(obj.scor);
    }
    return *this;
}

void Game::addObserver(Observer* observer) {
    observers.push_back(observer);
}

void Game::removeObserver(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Game::notifyObservers() {
    for (Observer* observer : observers) {
        observer->update();
    }
}
