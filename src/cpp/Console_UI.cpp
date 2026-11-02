//
// Created by Alex on 20.11.2024.
//

#include "headers/Console_UI.hpp"

#include <algorithm>
#include <array>
#include <iostream>

#include "headers/Game.hpp"
#include "headers/Hint_Game.hpp"
#include "headers/Word_Manager.hpp"

ConsoleUI::ConsoleUI() {
    leaderboard.incarca();
}

std::optional<std::string> ConsoleUI::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        return std::nullopt; // EOF sau eroare de flux -> iesim curat, fara bucle infinite
    }
    return line;
}

std::optional<int> ConsoleUI::readChoice(const std::string& prompt, int min, int max) {
    while (true) {
        const auto line = readLine(prompt);
        if (!line) {
            return std::nullopt;
        }
        try {
            std::size_t used = 0;
            const int value = std::stoi(*line, &used);
            if (used == line->size() && value >= min && value <= max) {
                return value;
            }
        } catch (const std::exception&) {
            // nu e numar -> intrebam din nou
        }
        std::cout << "Optiune invalida, alege un numar intre " << min << " si " << max << ".\n";
    }
}

void ConsoleUI::drawHangman(int mistakes) {
    const std::array<const char*, 7> head = {" ", "O", "O", "O", "O", "O", "O"};
    const std::string body = mistakes >= 2 ? "|" : " ";
    const std::string leftArm = mistakes >= 3 ? "/" : " ";
    const std::string rightArm = mistakes >= 4 ? "\\" : " ";
    const std::string leftLeg = mistakes >= 5 ? "/" : " ";
    const std::string rightLeg = mistakes >= 6 ? "\\" : " ";

    std::cout << "  +---+\n"
              << "  |   |\n"
              << "  |   " << head[static_cast<std::size_t>(std::clamp(mistakes, 0, 6))] << "\n"
              << "  |  " << leftArm << body << rightArm << "\n"
              << "  |  " << leftLeg << " " << rightLeg << "\n"
              << "  |\n"
              << "=====\n";
}

void ConsoleUI::displayStatus(const Game& game) {
    drawHangman(game.getMistakes());
    std::cout << "Cuvant: ";
    for (char c : game.getGuessedWord()) {
        std::cout << c << ' ';
    }
    std::cout << "\nIncercari ramase: " << game.getAttemptsRemaining()
              << " | Hint-uri: " << game.getHintGame().getHintsRemaining();
    if (game.getTimer().hasLimit()) {
        std::cout << " | Timp: " << game.getTimer().getTimeRemaining() << "s";
    }
    if (!game.getWrongLetters().empty()) {
        std::cout << "\nLitere gresite: ";
        for (char c : game.getWrongLetters()) {
            std::cout << c << ' ';
        }
    }
    std::cout << "\n";
}

bool ConsoleUI::playRound(const std::string& playerName) {
    const auto category = readChoice(
        "\nSelecteaza o categorie:\n 1. Animale\n 2. Tehnologie\n 3. Geografie\n 4. Mixt\nAlegerea ta: ", 1, 4);
    if (!category) return false;
    const auto level = readChoice("Dificultate: 1. Usor  2. Normal  3. Greu\nAlegerea ta: ", 1, 3);
    if (!level) return false;

    const std::array<Category, 4> categories = {Category::Animals, Category::Technology, Category::Geography,
                                                Category::Mixed};
    const std::array<Difficulty, 3> levels = {Difficulty::easy(), Difficulty::normal(), Difficulty::hard()};
    const Difficulty& difficulty = levels[static_cast<std::size_t>(*level - 1)];

    Game game(createWordManager(categories[static_cast<std::size_t>(*category - 1)]),
              std::make_unique<HintGame>(difficulty.hints), difficulty);

    std::cout << "\nComenzi: o litera, cuvantul intreg, '?' pentru hint.\n";
    while (!game.isGameOver()) {
        displayStatus(game);
        const auto input = readLine("Ghiceste o litera sau cuvant: ");
        if (!input) return false;

        if (*input == "?") {
            if (const auto letter = game.useHint()) {
                std::cout << ">> Hint: litera '" << *letter << "' a fost dezvaluita.\n";
            } else {
                std::cout << ">> Nu mai ai hint-uri.\n";
            }
            continue;
        }

        switch (game.guessWord(*input)) {
            case GuessResult::Correct:        std::cout << ">> Corect!\n"; break;
            case GuessResult::Wrong:          std::cout << ">> Gresit.\n"; break;
            case GuessResult::AlreadyGuessed: std::cout << ">> Ai incercat deja aceasta litera.\n"; break;
            case GuessResult::Invalid:        std::cout << ">> Introdu doar litere.\n"; break;
            case GuessResult::GameFinished:   break;
        }
    }

    displayStatus(game);
    if (game.isGameWon()) {
        std::cout << "\nFelicitari! Ai ghicit cuvantul: " << game.getSelectedWord()
                  << " (+" << game.computeScore() << " puncte)\n";
    } else if (game.isTimeUp()) {
        std::cout << "\nTimpul a expirat! Cuvantul era: " << game.getSelectedWord() << "\n";
    } else {
        std::cout << "\nAi pierdut! Cuvantul era: " << game.getSelectedWord() << "\n";
    }

    leaderboard.actualizeazaScor(playerName, game.computeScore());
    leaderboard.salveaza();
    return true;
}

int ConsoleUI::run() {
    std::cout << "=== SPANZURATOAREA ===\n";
    auto name = readLine("Numele tau: ");
    if (!name) return 0;
    if (name->empty()) *name = "Anonim";

    while (playRound(*name)) {
        leaderboard.afiseazaLeaderboard();
        const auto again = readLine("\nMai joci? (d/n): ");
        if (!again || again->empty() || (again->front() != 'd' && again->front() != 'D')) {
            break;
        }
    }
    std::cout << "La revedere!\n";
    return 0;
}
