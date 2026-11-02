//
// Created by Alex on 20.11.2024.
//

#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include <optional>
#include <string>

#include "Leaderboard.hpp"
#include "User_Interface.hpp"

class Game;

// Varianta text a jocului (pornita cu --console sau cand nu exista GUI).
class ConsoleUI : public UserInterface {
private:
    Leaderboard leaderboard;

    static std::optional<std::string> readLine(const std::string& prompt);
    static std::optional<int> readChoice(const std::string& prompt, int min, int max);
    static void drawHangman(int mistakes);
    static void displayStatus(const Game& game);
    bool playRound(const std::string& playerName);

public:
    ConsoleUI();
    int run() override;
};

#endif //CONSOLE_UI_HPP
