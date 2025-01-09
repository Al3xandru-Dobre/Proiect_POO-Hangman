#include "Hangman_UI.hpp"
#include "Game.hpp"
#include "Hint_Game.hpp"
#include <iostream>

//Constructor: receives configured instances of Game and HintGame
HangmanUI::HangmanUI(Game& gameInstance, HintGame* hintInstance)
    : game(gameInstance), hintGame(*hintInstance) {
    std::cout << "HangmanUI: Constructor apelat cu HintGame: "
              << (hintInstance != nullptr ? "Valid" : "Nullptr") << "\n";
    game.addObserver(this);
}

void HangmanUI::start() {
    std::string input;
    std::cout << "Bine ai venit la jocul de Spanzuratoarea!\n";
    update();
    try {
        while (!game.isGameOver() && !game.isGameWon()) {
            game.displayStatus();

            std::cout << "Introdu o litera sau un cuvant complet (sau scrie 'Hint' pentru un indiciu): ";
            std::cin >> input;

            if (input == "Hint" || input == "hint") {
                hintGame.offerHintDirect();
                continue; // Ignores the input in order to not trigger the guess
            }

            bool correctGuess;
            if (input.size() == 1) { //Logica pentru o singura litera introdusa
                char letter = input[0];
                correctGuess = game.guessLetter(letter);
            } else { //Logica pentru cuvantul introdus
                correctGuess = game.guessWord(input);
            }

            if (correctGuess) {
                std::cout << "Ai ghicit corect!\n";
            } else {
                std::cout << "Nu ai ghicit corect.\n";
            }
        }

        if (game.isGameWon()) {
            std::cout << "Felicitari! Ai ghicit cuvantul: " << game.getGuessedWord() << "\n";
            game->afiseazaScor();
        } else {
            std::cout << "Ai pierdut! Cuvantul era: " << game.getSelectedWord() << "\n";
            game->afiseazaScor();
        }
    } catch (const std::exception& ex) {
        std::cerr << "Eroare: " << ex.what() << "\n";
    }
    std::cout << "Jocul s-a terminat.\n";
}

[[nodiscard]] bool HangmanUI::gameWon() const {
    return game.isGameWon();
}


Game* HangmanUI::operator->() {
    return &game; // Returnează pointerul brut către obiectul `Scor`
}
/*
    Am suprascris operatorul pentru a putea acceasa memmbrii lui Game in interiorul lui HangmanUI
    Fiindca am suprascris si operatorul in Game pt scor, prin Game::operator->()-> pot merge in in scor si apoi in Leaderboard
*/
// Operator -> pentru acces la metodele const din Scor
const Game* HangmanUI::operator->() const {
    return &game; // Returnează pointerul brut către obiectul `Scor`
}

// Move constructor
HangmanUI::HangmanUI(HangmanUI&& obj) noexcept
    : game(obj.game), hintGame(obj.hintGame) {
    //obj.hintGame = nullptr;
}

// Move assignment operator
HangmanUI& HangmanUI::operator=(HangmanUI&& obj) noexcept {
    if (this != &obj) {  // Verificăm auto-asignarea
        // Eliberăm resursele existente (dacă e nevoie)
        //game = nullptr;       // sau delete game dacă este un pointer brut
        //hintGame = nullptr;

        // Mutăm resursele din obiectul sursă
        game = std::move(obj.game);
        hintGame = std::move(obj.hintGame);

        // Resetăm obiectul sursă
        //obj.game = nullptr;
       // obj.hintGame = nullptr;
    }
    return *this;
}