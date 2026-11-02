//
// Created by Alex on 20.11.2024.
//

#include "headers/ui/MenuScene.hpp"

#include <array>
#include <cmath>

#include "headers/Hangman_UI.hpp"
#include "headers/Random.hpp"
#include "headers/ui/Style.hpp"

namespace ui {

namespace {
constexpr float PanelLeft = 600.f, PanelTop = 160.f, PanelWidth = 520.f, PanelHeight = 560.f;
constexpr float ColumnCenter = PanelLeft + PanelWidth / 2.f;
constexpr float ContentWidth = 440.f;

const std::array<Category, 4> Categories = {Category::Animals, Category::Technology, Category::Geography,
                                            Category::Mixed};
const std::array<const char*, 4> CategoryNames = {"Animale", "Tehnologie", "Geografie", "Mixt"};
const std::array<const char*, 3> DifficultyNames = {"Ușor", "Normal", "Greu"};

sf::Text makeLabel(const sf::Font& font, const char* text, float x, float y) {
    sf::Text label(utf8(text), font, 15);
    label.setFillColor(theme::TextMuted);
    label.setLetterSpacing(1.6f);
    label.setPosition(x, y);
    return label;
}
} // namespace

MenuScene::MenuScene(HangmanUI& app)
    : Scene(app),
      panel({PanelWidth, PanelHeight}, 22.f),
      nameField(app.getFont(), {ContentWidth, 50.f}, 16, utf8("Scrie numele tău...")),
      playButton(app.getBoldFont(), utf8("JOACĂ"), {ContentWidth, 64.f}, 28),
      leaderboardButton(app.getFont(), utf8("Clasament"), {(ContentWidth - 12.f) / 2.f, 50.f}, 20),
      quitButton(app.getFont(), utf8("Ieșire"), {(ContentWidth - 12.f) / 2.f, 50.f}, 20) {
    const sf::Font& font = app.getFont();
    const sf::Font& bold = app.getBoldFont();
    const float left = ColumnCenter - ContentWidth / 2.f;

    title = sf::Text(utf8("SPÂNZURĂTOAREA"), bold, 64);
    title.setLetterSpacing(1.3f);
    title.setFillColor(theme::Text);
    centerOrigin(title);
    title.setPosition(theme::WindowWidth / 2.f, 72.f);

    subtitle = sf::Text(utf8("Proiect POO  ·  C++20  ·  SFML"), font, 18);
    subtitle.setFillColor(theme::Accent);
    subtitle.setLetterSpacing(1.5f);
    centerOrigin(subtitle);
    subtitle.setPosition(theme::WindowWidth / 2.f, 124.f);

    panel.setPosition(PanelLeft, PanelTop);
    panel.setFillColor(withAlpha(theme::Panel, 0.92f));
    panel.setOutlineThickness(1.5f);
    panel.setOutlineColor(theme::Border);

    nameLabel = makeLabel(font, "NUME JUCĂTOR", left, 188.f);
    nameField.setPosition(ColumnCenter, 238.f);
    nameField.setText(app.getSettings().playerName);
    nameField.setOnSubmit([this](const std::string&) { play(); });

    categoryLabel = makeLabel(font, "CATEGORIE", left, 284.f);
    const float categoryWidth = (ContentWidth - 3.f * 8.f) / 4.f;
    for (std::size_t i = 0; i < Categories.size(); ++i) {
        auto button = std::make_unique<Button>(font, utf8(CategoryNames[i]), sf::Vector2f{categoryWidth, 48.f}, 15);
        button->setPosition(left + categoryWidth / 2.f + static_cast<float>(i) * (categoryWidth + 8.f), 334.f);
        button->setOnClick([this, i] { selectCategory(i); });
        categoryButtons.push_back(std::move(button));
    }

    difficultyLabel = makeLabel(font, "DIFICULTATE", left, 380.f);
    const float difficultyWidth = (ContentWidth - 2.f * 8.f) / 3.f;
    for (std::size_t i = 0; i < DifficultyNames.size(); ++i) {
        auto button = std::make_unique<Button>(font, utf8(DifficultyNames[i]), sf::Vector2f{difficultyWidth, 48.f}, 18);
        button->setPosition(left + difficultyWidth / 2.f + static_cast<float>(i) * (difficultyWidth + 8.f), 430.f);
        button->setOnClick([this, i] { selectDifficulty(i); });
        difficultyButtons.push_back(std::move(button));
    }
    difficultyInfo = sf::Text("", font, 16);
    difficultyInfo.setFillColor(theme::TextMuted);

    playButton.setPalette(theme::AccentDark, sf::Color(14, 130, 180), theme::Text);
    playButton.setPosition(ColumnCenter, 560.f);
    playButton.setOnClick([this] { play(); });

    leaderboardButton.setPosition(left + leaderboardButton.getSize().x / 2.f, 646.f);
    leaderboardButton.setOnClick([this] { this->app.showLeaderboard(); });
    quitButton.setPosition(left + ContentWidth - quitButton.getSize().x / 2.f, 646.f);
    quitButton.setOnClick([this] { this->app.quit(); });

    footer = sf::Text(utf8("Enter = joacă   ·   Esc = ieșire   ·   tastele 1-4 / săgețile schimbă categoria"), font, 14);
    footer.setFillColor(withAlpha(theme::TextMuted, 0.7f));
    centerOrigin(footer);
    footer.setPosition(theme::WindowWidth / 2.f, 740.f);

    // Restauram alegerile anterioare.
    const auto& settings = app.getSettings();
    for (std::size_t i = 0; i < Categories.size(); ++i) {
        if (Categories[i] == settings.category) selectCategory(i);
    }
    selectDifficulty(static_cast<std::size_t>(settings.difficultyIndex));

    figure.setPosition(130.f, 210.f);
    figure.buildGallows(0.3f);
    figure.setMistakes(6);

    // Litere care plutesc discret in fundal.
    for (int i = 0; i < 26; ++i) {
        const char letter = static_cast<char>('A' + i);
        FloatingLetter floating{sf::Text(sf::String(letter), bold, static_cast<unsigned>(Random::between(22, 64))),
                                Random::realBetween(8.f, 26.f), Random::realBetween(-12.f, 12.f)};
        floating.text.setFillColor(withAlpha(theme::Accent, Random::realBetween(0.04f, 0.12f)));
        centerOrigin(floating.text);
        floating.text.setPosition(Random::realBetween(0.f, theme::WindowWidth), Random::realBetween(0.f, theme::WindowHeight));
        floating.text.setRotation(Random::realBetween(-25.f, 25.f));
        letters.push_back(std::move(floating));
    }
}

void MenuScene::selectCategory(std::size_t index) {
    for (std::size_t i = 0; i < categoryButtons.size(); ++i) categoryButtons[i]->setSelected(i == index);
    app.getSettings().category = Categories[index];
}

void MenuScene::selectDifficulty(std::size_t index) {
    for (std::size_t i = 0; i < difficultyButtons.size(); ++i) difficultyButtons[i]->setSelected(i == index);
    app.getSettings().difficultyIndex = static_cast<int>(index);
    refreshDifficultyText();
}

void MenuScene::refreshDifficultyText() {
    const Difficulty d = app.getSettings().difficulty();
    const std::string info = std::to_string(d.hints) + (d.hints == 1 ? " indiciu" : " indicii") + "   ·   " +
                             std::to_string(d.timeLimitSeconds) + " secunde   ·   scor ×" +
                             std::to_string(d.scoreMultiplier);
    difficultyInfo.setString(utf8(info));
    centerOrigin(difficultyInfo);
    difficultyInfo.setPosition(ColumnCenter, 482.f);
}

void MenuScene::play() {
    std::string name = nameField.getText();
    while (!name.empty() && name.back() == ' ') name.pop_back();
    if (name.empty()) {
        nameField.shake();
        nameField.setFocused(true);
        app.notify(utf8("Introdu un nume înainte de a juca"), theme::Gold);
        return;
    }
    app.getSettings().playerName = name;
    app.startNewGame();
}

void MenuScene::handleEvent(const sf::Event& event, sf::Vector2f mouse) {
    if (nameField.handleEvent(event, mouse)) return;

    for (auto& button : categoryButtons) button->handleEvent(event, mouse);
    for (auto& button : difficultyButtons) button->handleEvent(event, mouse);
    playButton.handleEvent(event, mouse);
    leaderboardButton.handleEvent(event, mouse);
    quitButton.handleEvent(event, mouse);

    if (event.type == sf::Event::KeyPressed) {
        const auto& settings = app.getSettings();
        std::size_t category = 0;
        for (std::size_t i = 0; i < Categories.size(); ++i) {
            if (Categories[i] == settings.category) category = i;
        }
        switch (event.key.code) {
            case sf::Keyboard::Enter:  play(); break;
            case sf::Keyboard::Escape: app.quit(); break;
            case sf::Keyboard::Right:  selectCategory((category + 1) % Categories.size()); break;
            case sf::Keyboard::Left:   selectCategory((category + Categories.size() - 1) % Categories.size()); break;
            case sf::Keyboard::Up:
                selectDifficulty(static_cast<std::size_t>(std::max(0, settings.difficultyIndex - 1)));
                break;
            case sf::Keyboard::Down:
                selectDifficulty(static_cast<std::size_t>(std::min(2, settings.difficultyIndex + 1)));
                break;
            case sf::Keyboard::Num1: case sf::Keyboard::Num2: case sf::Keyboard::Num3: case sf::Keyboard::Num4:
                selectCategory(static_cast<std::size_t>(event.key.code - sf::Keyboard::Num1));
                break;
            default: break;
        }
    }
}

void MenuScene::update(float dt) {
    time += dt;
    figure.update(dt);
    if (!waved && time > 4.6f) { // dupa ce s-a desenat, omuletul face cu mana
        figure.setMood(FigureMood::Won);
        waved = true;
    }
    nameField.update(dt);
    for (auto& button : categoryButtons) button->update(dt);
    for (auto& button : difficultyButtons) button->update(dt);
    playButton.update(dt);
    leaderboardButton.update(dt);
    quitButton.update(dt);

    // Titlul "respira" usor, iar butonul JOACA pulseaza discret.
    const float bob = std::sin(time * 1.6f) * 4.f;
    title.setPosition(theme::WindowWidth / 2.f, 72.f + bob);
    const float pulse = 1.f + 0.015f * std::sin(time * 3.2f);
    playButton.setScale(pulse, pulse);

    for (auto& floating : letters) {
        floating.text.move(0.f, -floating.speed * dt);
        floating.text.rotate(floating.spin * dt);
        if (floating.text.getPosition().y < -60.f) {
            floating.text.setPosition(Random::realBetween(0.f, theme::WindowWidth), theme::WindowHeight + 60.f);
        }
    }
}

void MenuScene::draw(sf::RenderTarget& target) const {
    for (const auto& floating : letters) target.draw(floating.text);

    // umbra titlului pentru adancime
    sf::Text shadow = title;
    shadow.setFillColor(sf::Color(0, 0, 0, 110));
    shadow.move(0.f, 5.f);
    target.draw(shadow);
    target.draw(title);
    target.draw(subtitle);

    target.draw(figure);
    target.draw(panel);
    target.draw(nameLabel);
    target.draw(nameField);
    target.draw(categoryLabel);
    for (const auto& button : categoryButtons) target.draw(*button);
    target.draw(difficultyLabel);
    for (const auto& button : difficultyButtons) target.draw(*button);
    target.draw(difficultyInfo);
    target.draw(playButton);
    target.draw(leaderboardButton);
    target.draw(quitButton);
    target.draw(footer);
}

} // namespace ui
