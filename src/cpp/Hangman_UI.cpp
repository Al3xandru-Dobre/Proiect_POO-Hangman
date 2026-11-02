//
// Created by Alex on 20.11.2024.
//

#include "headers/Hangman_UI.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "headers/Hint_Game.hpp"
#include "headers/Resources.hpp"
#include "headers/ui/GameScene.hpp"
#include "headers/ui/LeaderboardScene.hpp"
#include "headers/ui/MenuScene.hpp"
#include "headers/ui/Style.hpp"

using namespace ui;

Difficulty GameSettings::difficulty() const {
    switch (difficultyIndex) {
        case 0:  return Difficulty::easy();
        case 2:  return Difficulty::hard();
        default: return Difficulty::normal();
    }
}

HangmanUI::HangmanUI() : toasts(font) {
    if (!font.loadFromFile(resolveResourcePath("assets/fonts/DejaVuSans.ttf")) ||
        !boldFont.loadFromFile(resolveResourcePath("assets/fonts/DejaVuSans-Bold.ttf"))) {
        throw std::runtime_error("Nu pot incarca fonturile din assets/fonts");
    }

    sf::ContextSettings context;
    context.antialiasingLevel = 8; // margini netede pentru linii si cercuri
    window.create(sf::VideoMode(static_cast<unsigned>(theme::WindowWidth), static_cast<unsigned>(theme::WindowHeight)),
                  utf8("Spânzurătoarea — Proiect POO"), sf::Style::Titlebar | sf::Style::Close, context);
    window.setFramerateLimit(60);

    leaderboard.incarca();
    toasts.setPosition(theme::WindowWidth / 2.f, 40.f);
    buildBackground();
    currentScene = std::make_unique<MenuScene>(*this);
}

HangmanUI::~HangmanUI() = default;

void HangmanUI::buildBackground() {
    // Gradient vertical (4 varfuri, culori interpolate de placa grafica).
    background = sf::VertexArray(sf::TriangleStrip, 4);
    background[0] = sf::Vertex({0.f, 0.f}, theme::BackgroundTop);
    background[1] = sf::Vertex({theme::WindowWidth, 0.f}, theme::BackgroundTop);
    background[2] = sf::Vertex({0.f, theme::WindowHeight}, theme::Background);
    background[3] = sf::Vertex({theme::WindowWidth, theme::WindowHeight}, theme::Background);
}

const sf::Font& HangmanUI::getFont() const { return font; }
const sf::Font& HangmanUI::getBoldFont() const { return boldFont; }
Leaderboard& HangmanUI::getLeaderboard() { return leaderboard; }
GameSettings& HangmanUI::getSettings() { return settings; }

void HangmanUI::changeScene(std::unique_ptr<Scene> next) {
    pendingScene = std::move(next);
    fadingOut = true;
}

void HangmanUI::startNewGame() {
    try {
        const Difficulty difficulty = settings.difficulty();
        auto game = std::make_unique<Game>(createWordManager(settings.category),
                                           std::make_unique<HintGame>(difficulty.hints), difficulty);
        changeScene(std::make_unique<GameScene>(*this, std::move(game)));
    } catch (const std::exception& error) {
        notify(utf8(error.what()), theme::Danger);
    }
}

void HangmanUI::showMenu() { changeScene(std::make_unique<MenuScene>(*this)); }
void HangmanUI::showLeaderboard() { changeScene(std::make_unique<LeaderboardScene>(*this)); }
void HangmanUI::notify(const sf::String& message, sf::Color accent) { toasts.push(message, accent); }
void HangmanUI::quit() { window.close(); }

void HangmanUI::applyPendingScene(float dt) {
    // Tranzitie: fade-out rapid (0.2 s) -> schimbam scena -> fade-in (0.3 s).
    if (fadingOut) {
        fade = std::min(1.f, fade + dt / 0.2f);
        if (fade >= 1.f && pendingScene) {
            currentScene = std::move(pendingScene);
            fadingOut = false;
        }
    } else {
        fade = std::max(0.f, fade - dt / 0.3f);
    }
}

int HangmanUI::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        // dt limitat: daca fereastra este mutata/blocata, animatiile nu "sar".
        const float dt = std::min(clock.restart().asSeconds(), 1.f / 20.f);

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
            if (!fadingOut && currentScene) {
                const sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                currentScene->handleEvent(event, mouse);
            }
        }
        if (!window.isOpen()) break;

        applyPendingScene(dt);
        currentScene->update(dt);
        toasts.update(dt);

        window.clear(theme::Background);
        window.draw(background);
        currentScene->draw(window);
        window.draw(toasts);
        if (fade > 0.f) {
            sf::RectangleShape curtain({theme::WindowWidth, theme::WindowHeight});
            curtain.setFillColor(withAlpha(theme::Background, ease::inOutCubic(fade)));
            window.draw(curtain);
        }
        window.display();
    }
    leaderboard.salveaza();
    return 0;
}
