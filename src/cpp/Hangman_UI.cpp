<<<<<<< HEAD
#include "headers/Hangman_UI.hpp"
#include <iostream>

//Constructor: receives configured instances of Game and HintGame
HangmanUI::HangmanUI(Game& gameInstance, HintGame* hintInstance)
    : game(gameInstance), hintGame(*hintInstance) {}

// Starts the game
void HangmanUI::start() {
    std::string input;
    std::cout << "Bine ai venit la jocul de Spanzuratoarea!\n";

    try {
        while (!game.isGameOver() && !game.isGameWon()) {
            game.displayStatus(); // Display the progress of the game
            std::cout << "Introdu o litera sau un cuvant complet (sau scrie 'Hint' pentru un indiciu): ";
            std::cin >> input;

            if (input == "Hint" || input == "hint") {
                hintGame.offerHintDirect();
                continue; // Ignores the input in order to not trigger the guess
            }

            bool correctGuess;
            if (input.size() == 1) { // a single letter
                char letter = input[0];
                correctGuess = game.guessLetter(letter);
            } else { // It is a whole word
                correctGuess = game.guessWord(input);
=======
//
// Created by Alex on 20.11.2024.
//

#include "headers/Hangman_UI.hpp"
#include <iostream>
#include "headers/Word_Manager.hpp"

    HangmanUI::HangmanUI() : Game(std::make_unique<HintGame>(),std::make_unique<DefaultWordManager>(), 6) {
        HintGame game(3);
        game.setHints({"Primul hint", "Al doilea hint", "Al treilea hint"});
    }

    void HangmanUI::start(HintGame game) {
        std::string input;
        std::cout << "Bine ai venit la jocul de Spanzuratoarea!\n";

        while (!Game.isGameOver() && !Game.isGameWon()) {
            Game.displayStatus();
            std::cout << "Introdu o litera sau un cuvant complet: ";
            std::cin >> input;

            bool correctGuess;
            if (input.size() == 1) { // Este o literă
                char letter = input[0];
                correctGuess = Game.guessLetter(letter);
            } else { // Este un cuvânt întreg
                correctGuess = Game.guessWord(input);
>>>>>>> a1039bfa419783e09da6ceef0121c2b9e26aabc5
            }

            if (correctGuess) {
                std::cout << "Ai ghicit corect!\n";
            } else {
                std::cout << "Nu ai ghicit corect.\n";
<<<<<<< HEAD
            }
        }

        if (game.isGameWon()) {
            std::cout << "Felicitari! Ai ghicit cuvantul: " << game.getGuessedWord() << "\n";
        } else {
            std::cout << "Ai pierdut! Cuvantul era: " << game.getSelectedWord() << "\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Eroare: " << ex.what() << "\n";
    }
}

// Check if the game has been won
[[nodiscard]] bool HangmanUI::gameWon() const {
    return game.isGameWon();
}
=======
                game.requestHintOnIncorrectGuess(); // Oferă hint după un răspuns greșit, dacă e disponibil
            }
        }

        if (Game.isGameWon()) {
            std::cout << "Felicitari! Ai ghicit cuvantul: " << Game.getGuessedWord() << "\n";
        } else {
            std::cout << "Ai pierdut! Cuvantul era: " << Game.getSelectedWord() << "\n"; // Using the getter method
        }
    }

    bool HangmanUI::gameWon() const {
        return Game.isGameWon();
    }
>>>>>>> a1039bfa419783e09da6ceef0121c2b9e26aabc5
