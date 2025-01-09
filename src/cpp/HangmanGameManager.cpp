#include "HangmanGameManager.hpp"
#include "invalidInputException.hpp"
#include <limits>
#include "WordManagerFacotry.hpp"
#include "DefaultWordManager.hpp"

// Constructor
HangmanGameManager::HangmanGameManager()
    : leaderboard("leaderboard.txt"),game(){
    // Cerem numele jucătorului
    std::string nume;
    std::cout << "Introdu numele tau: ";
    std::cin >> nume;

    if (std::cin.fail()) {
        throw InvalidInputException("Eroare la citirea numelui!");
    }

    jucator = std::make_unique<Scor>(nume);  // Inițializare jucător

    selecteazaCategorie();
    if(!wordManager) {
        throw std::runtime_error("WordManager nu a fost initializat inainte de HintGame!");
    }
    std::cout << "WordManager creat corect pentru categoria aleasă.\n";

    // Inițializăm hintGame și game
    hintGame = std::make_unique<HintGame>(3, wordManager.get());  // Inițializare HintGame
    game = Game(std::move(hintGame), std::move(wordManager), std::move(jucator), 3);  // Inițializare Game

    if (!game.getHintGame()) {
        throw std::runtime_error("HintGame nu este valid!");
    }
    ui = std::make_unique<HangmanUI>(game, game.getHintGame());

}

// Metodă pentru a porni jocul
/*In aceasta metoda am folosit operatorii suprascrisi pentru fiecare dintre clasele participante
 *Practic acesta clasa este un manager care se ocupa de initializarea tuturor claselor si de a porni jocul
 *Partile sale componente sunt responsabile de functionalitatea jocului
 *ui->start() se ocupa de desfasurarea jocului
 *leaderboard-ul este initializat in constructor si trimis catre ui pentru a putea fi folosit in joc
 *mai jos se salveaza scorurile in leaderboard din ui->game->scor->leaderboard, teoretic puteam folosi direct sageata si sa merg in leaderboard, dar am ales sa aplic asa pentru a fi mai explicit,si pentru ca prima oara nu aveam toti operatorii suprascirsi si nu mergea altfel
 */
void HangmanGameManager::startJoc() {
    ui->start();
    ui->operator->()->operator->()->trimiteScorLaLeaderboard(leaderboard); // Ensure game is an object, not a pointer
    leaderboard.salveazaScoruriInFisier();
}

void HangmanGameManager::selecteazaCategorie() {
    int choice;
    std::cout << "Alege categoria cu care vrei să joci:\n";
    std::cout << "1. Animale\n";
    std::cout << "2. Tehnologie\n";
    std::cout << "3. Geografie\n";
    std::cout << "Introdu alegerea ta: ";

    if (!(std::cin >> choice)) {
        std::cin.clear(); // Resetăm flag-urile de eroare
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Golim buffer-ul
        std::cout << "Input invalid! Alegem managerul default.\n";
        wordManager = std::make_unique<DefaultWordManager>();
        return;
    }

    try {
        wordManager = WordManagerFacotry::createWordManager(choice);
        std::cout << "Categorie selectată cu succes.\n";
    } catch(const InvalidInputException& e) {
        std::cout <<"Eroare: " << e.what() << ".Folosimt WordManager default"<<std::endl;
        wordManager = std::make_unique<DefaultWordManager>();
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
