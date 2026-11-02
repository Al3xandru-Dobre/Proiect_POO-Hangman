//
// Created by Alex on 20.11.2024.
//

#include "headers/Game.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>

#include "headers/Hint_Game.hpp"
#include "headers/Word_Manager.hpp"

Difficulty Difficulty::easy()   { return {"Usor",   6, 3, 150, 1}; }
Difficulty Difficulty::normal() { return {"Normal", 6, 2, 90,  2}; }
Difficulty Difficulty::hard()   { return {"Greu",   6, 1, 60,  3}; }

Game::Game(std::unique_ptr<WordManager> manager, std::unique_ptr<HintGame> hintManager, Difficulty settings)
    : wordManager(std::move(manager)),
      hintGame(std::move(hintManager)),
      difficulty(std::move(settings)),
      timer(difficulty.timeLimitSeconds),
      attemptsRemaining(difficulty.maxAttempts) {
    if (!wordManager || !hintGame) {
        throw std::invalid_argument("Game are nevoie de un WordManager si de un HintGame valide");
    }
    wordManager->selectRandomWordFromCategory();

    // Literele sunt ascunse; spatiile si cratimele sunt vizibile de la inceput.
    const std::string& secret = wordManager->getSelectedWord();
    guessedWord = secret;
    std::replace_if(guessedWord.begin(), guessedWord.end(),
                    [](char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0; }, '_');
    timer.restart(); // timpul porneste dupa ce cuvantul a fost incarcat
}

Game::Game(Game&&) noexcept = default;
Game& Game::operator=(Game&&) noexcept = default;
Game::~Game() = default;

int Game::revealLetter(char letter) {
    int revealed = 0;
    const std::string& secret = wordManager->getSelectedWord();
    for (std::size_t i = 0; i < secret.size(); ++i) {
        if (secret[i] == letter && guessedWord[i] == '_') {
            guessedWord[i] = letter;
            ++revealed;
        }
    }
    return revealed;
}

void Game::finishIfOver() {
    if (isGameOver()) {
        timer.stop();
    }
}

GuessResult Game::guessLetter(char letter) {
    update();
    if (isGameOver()) {
        return GuessResult::GameFinished;
    }

    const auto c = static_cast<unsigned char>(letter);
    if (!std::isalpha(c)) {
        return GuessResult::Invalid;
    }
    letter = static_cast<char>(std::tolower(c));

    if (!triedLetters.insert(letter).second) {
        return GuessResult::AlreadyGuessed; // repetarea nu se penalizeaza
    }

    if (wordManager->isLetterInWord(letter)) {
        revealLetter(letter);
        finishIfOver();
        return GuessResult::Correct;
    }

    wrongLetters.push_back(letter);
    --attemptsRemaining;
    finishIfOver();
    return GuessResult::Wrong;
}

GuessResult Game::guessWord(const std::string& word) {
    update();
    if (isGameOver()) {
        return GuessResult::GameFinished;
    }

    const std::string normalized = WordManager::normalizeWord(word);
    if (normalized.empty()) {
        return GuessResult::Invalid;
    }
    if (normalized.size() == 1) {
        return guessLetter(normalized[0]);
    }

    if (normalized == wordManager->getSelectedWord()) {
        guessedWord = normalized;
        finishIfOver();
        return GuessResult::Correct;
    }

    --attemptsRemaining;
    finishIfOver();
    return GuessResult::Wrong;
}

std::optional<char> Game::useHint() {
    update();
    if (isGameOver()) {
        return std::nullopt;
    }
    const auto letter = hintGame->provideHint(wordManager->getSelectedWord(), guessedWord);
    if (letter) {
        triedLetters.insert(*letter);
        revealLetter(*letter);
        finishIfOver();
    }
    return letter;
}

void Game::update() {
    if (!isGameOver() && timer.isTimeUp()) {
        timeExpired = true;
        timer.stop();
    }
}

bool Game::isGameWon() const {
    return guessedWord == wordManager->getSelectedWord();
}

bool Game::isGameLost() const {
    return !isGameWon() && (attemptsRemaining <= 0 || timeExpired);
}

bool Game::isGameOver() const {
    return isGameWon() || isGameLost();
}

bool Game::isTimeUp() const {
    return timeExpired;
}

int Game::getAttemptsRemaining() const { return attemptsRemaining; }
int Game::getMaxAttempts() const { return difficulty.maxAttempts; }
int Game::getMistakes() const { return difficulty.maxAttempts - attemptsRemaining; }
const std::string& Game::getGuessedWord() const { return guessedWord; }
const std::string& Game::getSelectedWord() const { return wordManager->getSelectedWord(); }
const std::vector<char>& Game::getWrongLetters() const { return wrongLetters; }
const HintGame& Game::getHintGame() const { return *hintGame; }
const WordManager& Game::getWordManager() const { return *wordManager; }
const GameTimer& Game::getTimer() const { return timer; }
const Difficulty& Game::getDifficulty() const { return difficulty; }

bool Game::wasLetterTried(char letter) const {
    return triedLetters.count(static_cast<char>(std::tolower(static_cast<unsigned char>(letter)))) > 0;
}

int Game::computeScore() const {
    if (!isGameWon()) {
        return 0;
    }
    const std::string& secret = wordManager->getSelectedWord();
    std::set<char> distinct;
    for (char c : secret) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            distinct.insert(c);
        }
    }

    int score = 10 * static_cast<int>(distinct.size()) // dificultatea cuvantului
              + 15 * attemptsRemaining                  // greseli evitate
              + timer.getTimeRemaining()                // viteza
              - 20 * hintGame->getHintsUsed();          // penalizare hint-uri
    score = std::max(score, 10);
    return score * difficulty.scoreMultiplier;
}
