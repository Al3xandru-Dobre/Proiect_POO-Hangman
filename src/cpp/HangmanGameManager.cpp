#include "HangmanGameManager.hpp"
#include "invalidInputException.hpp"
#include <limits>

// Constructor
HangmanGameManager::HangmanGameManager()
    : leaderboard("leaderboard.txt"), game() {
    // Cerem numele jucătorului
    std::string nume;
    std::cout << "Introdu numele tau: ";
    std::cin >> nume;

    if (std::cin.fail()) {
        throw InvalidInputException("Eroare la citirea numelui!");
    }

    jucator = std::make_unique<Scor>(nume);  // Inițializare jucător

    try {
        selecteazaCategorie();
    } catch (const InvalidInputException& e) {
        std::cerr << e.what() << std::endl;
        std::cout << "Se folosește categoria implicită.\n";
        wordManager = std::make_unique<DefaultWordManager>();  // Inițializare categorie implicită
    }

    // Inițializăm hintGame și game
    hintGame = std::make_unique<HintGame>(3, wordManager.get());
    game = Game(std::move(hintGame), std::move(wordManager), std::move(jucator), 3);  // Inițializare Game

    // Inițializăm UI
    ui : HangmanUI(game, game.getHintGame());  // Inițializare HangmanUI
}

// Metodă pentru a porni jocul
void HangmanGameManager::startJoc() {
    ui->start();
    ui->operator->()->operator->()->trimiteScorLaLeaderboard(leaderboard); // Ensure game is an object, not a pointer
    leaderboard.salveazaScoruriInFisier();
}

void HangmanGameManager::selecteazaCategorie() {
    int choice;
    std::cout << "Selectează o categorie:\n";
    std::cout << "1. Animale\n";
    std::cout << "2. Tehnologie\n";
    std::cout << "3. Geografie\n";
    std::cout << "Alegerea ta: ";

    if (!(std::cin >> choice)) {
        throw InvalidInputException("Input invalid! Trebuie să introduci un număr.");
    }

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
            throw InvalidInputException("Categorie invalidă! Trebuie să introduci un număr între 1 și 3.");
    }
}

Game* HangmanGameManager::operator->() {
    return &game; // Returnează pointerul brut către obiectul `Scor`
}
/*
    Am suprascris operatorul pentru a putea acceasa memmbrii lui Game in interiorul lui HangmanUI
    Fiindca am suprascris si operatorul in Game pt scor, prin Game::operator->()-> pot merge in in scor si apoi in Leaderboard
*/
// Operator -> pentru acces la metodele const din Scor
const Game* HangmanGameManager::operator->() const {
    return &game; // Returnează pointerul brut către obiectul `Scor`
}
