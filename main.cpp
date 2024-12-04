#include "src/headers/Leaderboard.hpp"
#include "src/headers/Game_Timer.hpp"
#include "src/headers/Hangman_UI.hpp"
#include "src/headers/Word_Manager.hpp"
#include "src/headers/Game.hpp"
#include <iostream>

int main() {

    std::string nume;
    std::cout << "Introdu numele tau: "; std::cin >> nume;
    Leaderboard leaderboard("leaderboard.txt");
    std::unique_ptr<Scor> jucator = std::make_unique<Scor>(nume);



    int choice;
    std::cout << "Selectează o categorie:\n";
    std::cout << "1. Animale\n";
    std::cout << "2. Tehnologie\n";
    std::cout << "3. Geografie\n";
    std::cout << "Alegerea ta: ";
    std::cin >> choice;

    std::unique_ptr<WordManager> wordManager;
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
            wordManager = std::make_unique<DefaultWordManager>();
    }

    auto hintGame = std::make_unique<HintGame>(3, wordManager.get());
    Game game(std::move(hintGame), std::move(wordManager), std::move(jucator),3);

    HangmanUI ui(game, game.getHintGame());
    ui.start();
    ui->Game::operator->()->trimiteScorLaLeaderboard();
    return 0;
}



