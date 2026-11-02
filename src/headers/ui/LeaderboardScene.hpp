//
// Created by Alex on 20.11.2024.
//

#ifndef UI_LEADERBOARD_SCENE_HPP
#define UI_LEADERBOARD_SCENE_HPP

#include <vector>

#include "Scene.hpp"
#include "Widgets.hpp"

namespace ui {

class LeaderboardScene : public Scene {
public:
    explicit LeaderboardScene(HangmanUI& app);

    void handleEvent(const sf::Event& event, sf::Vector2f mouse) override;
    void update(float dt) override;
    void draw(sf::RenderTarget& target) const override;

private:
    struct Row {
        sf::Text rank, name, points;
        sf::Color medal;
        bool highlighted;
    };

    RoundedRectangleShape panel;
    sf::Text title, emptyText;
    std::vector<Row> rows;
    Button backButton, playButton;
    float time = 0.f;
};

} // namespace ui

#endif //UI_LEADERBOARD_SCENE_HPP
