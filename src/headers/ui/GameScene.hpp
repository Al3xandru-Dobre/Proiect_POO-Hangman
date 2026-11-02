//
// Created by Alex on 20.11.2024.
//

#ifndef UI_GAME_SCENE_HPP
#define UI_GAME_SCENE_HPP

#include <memory>
#include <string>
#include <vector>

#include "HangmanFigure.hpp"
#include "ParticleSystem.hpp"
#include "Scene.hpp"
#include "WordWidgets.hpp"

class Game;
enum class GuessResult;

namespace ui {

// Ecranul de joc: View-ul pentru un obiect Game (Model). Scena traduce
// rezultatul fiecarei actiuni (GuessResult) in animatii si mesaje.
class GameScene : public Scene {
public:
    GameScene(HangmanUI& app, std::unique_ptr<Game> game);
    ~GameScene() override;

    void handleEvent(const sf::Event& event, sf::Vector2f mouse) override;
    void update(float dt) override;
    void draw(sf::RenderTarget& target) const override;

private:
    void guessLetter(char letter);
    void guessWord(const std::string& word);
    void useHint();
    void onWrongGuess();
    void refreshHud();
    void finishRound();

    std::unique_ptr<Game> game; // compozitie: scena detine runda curenta

    HangmanFigure figure;
    WordDisplay word;
    OnScreenKeyboard keyboard;
    TimerBar timerBar;
    TextField wordField;
    Button guessButton, hintButton, menuButton;
    ParticleSystem particles;
    RoundedRectangleShape figurePanel, header;
    sf::Text categoryText, playerText, wrongLabel, wrongLetters, attemptsLabel, footer;

    // ecranul de final de runda
    RoundedRectangleShape resultPanel;
    sf::Text resultTitle, resultWord, resultScore;
    Button againButton, resultBoardButton, resultMenuButton;

    bool roundOver = false;
    float resultDelay = 0.f;  // asteptam putin ca jucatorul sa vada animatia de final
    float resultT = 0.f;      // progresul animatiei panoului (0 -> 1)
    float shakeTime = 0.f;
    float redFlash = 0.f;
    float time = 0.f;
    float confettiTime = 0.f;
    std::vector<float> heartPulse; // cate un puls pentru fiecare incercare
    sf::Vector2f shakeOffset;
};

} // namespace ui

#endif //UI_GAME_SCENE_HPP
