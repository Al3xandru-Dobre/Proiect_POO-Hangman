//
// Created by Alex on 20.11.2024.
//

#ifndef HANGMAN_UI_HPP
#define HANGMAN_UI_HPP

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>

#include "Game.hpp"
#include "Leaderboard.hpp"
#include "User_Interface.hpp"
#include "Word_Manager.hpp"
#include "ui/Widgets.hpp"

namespace ui {
class Scene;
}

// Alegerile jucatorului din meniu, pastrate intre runde.
struct GameSettings {
    std::string playerName = "Jucator";
    Category category = Category::Mixed;
    int difficultyIndex = 1; // 0 = usor, 1 = normal, 2 = greu

    [[nodiscard]] Difficulty difficulty() const;
};

// Interfata grafica (SFML). Detine fereastra si resursele comune, ruleaza bucla
// principala si deleaga tot restul scenei active.
class HangmanUI : public UserInterface {
public:
    HangmanUI();
    ~HangmanUI() override; // definit in .cpp, unde ui::Scene este tip complet

    int run() override;

    // --- servicii oferite scenelor ---
    [[nodiscard]] const sf::Font& getFont() const;
    [[nodiscard]] const sf::Font& getBoldFont() const;
    [[nodiscard]] Leaderboard& getLeaderboard();
    [[nodiscard]] GameSettings& getSettings();

    void startNewGame();
    void showMenu();
    void showLeaderboard();
    void notify(const sf::String& message, sf::Color accent);
    void quit();

private:
    // Schimbarea scenei este AMANATA pana la finalul cadrului: scena curenta nu
    // poate fi distrusa in timp ce una dintre metodele ei inca ruleaza
    // (ex. callback-ul unui buton care a cerut schimbarea).
    void changeScene(std::unique_ptr<ui::Scene> next);
    void applyPendingScene(float dt);
    void buildBackground();

    sf::RenderWindow window;
    sf::Font font;
    sf::Font boldFont;
    Leaderboard leaderboard;
    GameSettings settings;
    ui::ToastStack toasts;
    sf::VertexArray background;

    std::unique_ptr<ui::Scene> currentScene;
    std::unique_ptr<ui::Scene> pendingScene;
    float fade = 1.f;          // 1 = ecran negru, 0 = transparent
    bool fadingOut = false;
};

#endif //HANGMAN_UI_HPP
