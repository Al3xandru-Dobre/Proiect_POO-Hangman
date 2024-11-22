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
            }

            if (correctGuess) {
                std::cout << "Ai ghicit corect!\n";
            } else {
                std::cout << "Nu ai ghicit corect.\n";
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