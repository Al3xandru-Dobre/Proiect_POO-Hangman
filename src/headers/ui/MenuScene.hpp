//
// Created by Alex on 20.11.2024.
//

#ifndef UI_MENU_SCENE_HPP
#define UI_MENU_SCENE_HPP

#include <memory>
#include <vector>

#include "HangmanFigure.hpp"
#include "Scene.hpp"
#include "Widgets.hpp"

namespace ui {

class MenuScene : public Scene {
public:
    explicit MenuScene(HangmanUI& app);

    void handleEvent(const sf::Event& event, sf::Vector2f mouse) override;
    void update(float dt) override;
    void draw(sf::RenderTarget& target) const override;

private:
    struct FloatingLetter {
        sf::Text text;
        float speed;
        float spin;
    };

    void selectCategory(std::size_t index);
    void selectDifficulty(std::size_t index);
    void play();
    void refreshDifficultyText();

    HangmanFigure figure;
    RoundedRectangleShape panel;
    sf::Text title, subtitle, nameLabel, categoryLabel, difficultyLabel, difficultyInfo, footer;
    TextField nameField;
    std::vector<std::unique_ptr<Button>> categoryButtons;
    std::vector<std::unique_ptr<Button>> difficultyButtons;
    Button playButton, leaderboardButton, quitButton;
    std::vector<FloatingLetter> letters;
    float time = 0.f;
    bool waved = false;
};

} // namespace ui

#endif //UI_MENU_SCENE_HPP
