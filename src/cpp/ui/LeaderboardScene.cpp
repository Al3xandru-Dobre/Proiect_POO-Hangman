//
// Created by Alex on 20.11.2024.
//

#include "headers/ui/LeaderboardScene.hpp"

#include "headers/Hangman_UI.hpp"
#include "headers/ui/Style.hpp"

namespace ui {

namespace {
constexpr float PanelWidth = 640.f;
constexpr float PanelTop = 150.f;
constexpr float RowsTop = 200.f;
constexpr float RowHeight = 44.f;
constexpr float CenterX = theme::WindowWidth / 2.f;
} // namespace

LeaderboardScene::LeaderboardScene(HangmanUI& app)
    : Scene(app),
      panel({PanelWidth, 500.f}, 24.f),
      backButton(app.getFont(), utf8("Înapoi"), {200.f, 52.f}, 19),
      playButton(app.getBoldFont(), utf8("Joacă"), {200.f, 52.f}, 19) {
    const sf::Font& font = app.getFont();
    const sf::Font& bold = app.getBoldFont();

    title = sf::Text(utf8("CLASAMENT"), bold, 52);
    title.setLetterSpacing(1.4f);
    title.setFillColor(theme::Text);
    centerOrigin(title);
    title.setPosition(CenterX, 90.f);

    panel.setPosition(CenterX - PanelWidth / 2.f, PanelTop);
    panel.setFillColor(withAlpha(theme::Panel, 0.92f));
    panel.setOutlineThickness(1.5f);
    panel.setOutlineColor(theme::Border);

    emptyText = sf::Text(utf8("Încă nu există scoruri. Joacă o rundă!"), font, 22);
    emptyText.setFillColor(theme::TextMuted);
    centerOrigin(emptyText);
    emptyText.setPosition(CenterX, PanelTop + 220.f);

    const sf::Color medals[3] = {theme::Gold, sf::Color(203, 213, 225), sf::Color(217, 139, 82)};
    const auto top = app.getLeaderboard().getTop(10);
    for (std::size_t i = 0; i < top.size(); ++i) {
        Row row{sf::Text(std::to_string(i + 1), bold, 20), sf::Text(utf8(top[i].getNumeJucator()), font, 21),
                sf::Text(std::to_string(top[i].getScorCurent()), bold, 21),
                i < 3 ? medals[i] : theme::PanelLight, top[i].getNumeJucator() == app.getSettings().playerName};
        row.rank.setFillColor(i < 3 ? theme::Background : theme::Text);
        centerOrigin(row.rank);
        row.name.setFillColor(theme::Text);
        row.points.setFillColor(i < 3 ? medals[i] : theme::Accent);
        const sf::FloatRect b = row.points.getLocalBounds();
        row.points.setOrigin(b.left + b.width, 0.f);
        rows.push_back(std::move(row));
    }

    backButton.setPosition(CenterX - 110.f, 700.f);
    backButton.setOnClick([this] { this->app.showMenu(); });
    playButton.setPalette(theme::AccentDark, sf::Color(14, 130, 180), theme::Text);
    playButton.setPosition(CenterX + 110.f, 700.f);
    playButton.setOnClick([this] { this->app.startNewGame(); });
}

void LeaderboardScene::handleEvent(const sf::Event& event, sf::Vector2f mouse) {
    backButton.handleEvent(event, mouse);
    playButton.handleEvent(event, mouse);
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Backspace) app.showMenu();
        if (event.key.code == sf::Keyboard::Enter) app.startNewGame();
    }
}

void LeaderboardScene::update(float dt) {
    time += dt;
    backButton.update(dt);
    playButton.update(dt);
}

void LeaderboardScene::draw(sf::RenderTarget& target) const {
    target.draw(title);
    target.draw(panel);
    if (rows.empty()) {
        target.draw(emptyText);
    }

    const float left = CenterX - PanelWidth / 2.f + 30.f;
    const float right = CenterX + PanelWidth / 2.f - 30.f;
    for (std::size_t i = 0; i < rows.size(); ++i) {
        // Randurile intra pe rand, de la dreapta, cu o usoara intarziere intre ele.
        const float t = ease::clamp01((time - 0.08f * static_cast<float>(i)) / 0.45f);
        const float slide = ease::lerp(80.f, 0.f, ease::outCubic(t));
        const float y = RowsTop + static_cast<float>(i) * RowHeight;
        sf::RenderStates states;
        states.transform.translate(slide, 0.f);

        const Row& row = rows[i];
        RoundedRectangleShape background({PanelWidth - 40.f, RowHeight - 6.f}, 10.f);
        background.setPosition(left - 10.f, y);
        background.setFillColor(withAlpha(row.highlighted ? theme::AccentDark : theme::PanelLight,
                                          (row.highlighted ? 0.9f : 0.45f) * t));
        target.draw(background, states);

        sf::CircleShape badge(15.f, 32);
        badge.setOrigin(15.f, 15.f);
        badge.setPosition(left + 18.f, y + (RowHeight - 6.f) / 2.f);
        badge.setFillColor(withAlpha(row.medal, t));
        target.draw(badge, states);

        sf::Text rank = row.rank, name = row.name, points = row.points;
        rank.setPosition(badge.getPosition());
        name.setPosition(left + 52.f, y + 6.f);
        points.setPosition(right - 6.f, y + 6.f);
        for (sf::Text* text : {&rank, &name, &points}) {
            text->setFillColor(withAlpha(text->getFillColor(), t));
            target.draw(*text, states);
        }
    }

    target.draw(backButton);
    target.draw(playButton);
}

} // namespace ui
