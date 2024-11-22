#include "src/headers/Leaderboard.hpp"
#include "src/headers/Game_Timer.hpp"
#include "src/headers/Hangman_UI.hpp"
#include "src/headers/Word_Manager.hpp"
#include "src/headers/Game.hpp"
#include "src/headers/Hint_Game.hpp"

int main() {
    try {
        int choice;
        std::cout << "Selecteaza o categorie:\n";
        std::cout << "1. Animale\n";
        std::cout << "2. Tehnologie\n";
        std::cout << "3. Geografie\n";
        std::cout << "Alegerea ta: ";
        std::cin >> choice;

        std::unique_ptr<WordManager> wordManager;
        std::vector<std::string> hints = {"Hint 1", "Hint 2", "Hint 3"};
        switch (choice) {
            case 1:
                wordManager = std::make_unique<AnimalWordManager>();
            break;
            case 2:
                wordManager = std::make_unique<TechnologyWordManager>();
            break;
            case 3:
                wordManager = std::make_unique<GeographyWordManager>();
            break;
            default:
                std::cout << "Categorie invalida! Jocul se va închide.\n";
            return 1;
        }

        auto hintGame = std::make_unique<HintGame>(3);
        hintGame->setHints(hints);

        Game game(std::move(hintGame),std::move(wordManager), 6);
        while (!game.isGameOver() && !game.isGameWon()) {
            game.displayStatus();
            std::cout << "Ghiceste o litera sau cuvant: ";

            std::string guess;
            std::cin >> guess;

            if (guess.size() == 1) {
                char letter = std::tolower(guess[0]);
                game.guessLetter(letter);
            } else {
                game.guessWord(guess);
            }
        }

        if (game.isGameWon()) {
            std::cout << "Felicitari! Ai ghicit cuvantul: " << game.getSelectedWord() << "\n";
        } else {
            std::cout << "Ai pierdut! Cuvantul era: " << game.getSelectedWord() << "\n";
        }

    } catch (const std::exception& ex) {
        std::cerr << "Eroare: " << ex.what() << "\n";
    }

    return 0;
}
