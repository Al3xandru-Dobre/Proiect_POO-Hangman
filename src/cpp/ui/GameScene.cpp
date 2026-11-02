//
// Created by Alex on 20.11.2024.
//

#include "headers/ui/GameScene.hpp"

#include <cctype>
#include <cmath>

#include "headers/Game.hpp"
#include "headers/Hangman_UI.hpp"
#include "headers/Hint_Game.hpp"
#include "headers/Word_Manager.hpp"
#include "headers/ui/Style.hpp"

namespace ui {

namespace {
constexpr float RightCenter = 800.f;
constexpr float WordY = 222.f;
constexpr float ResultRestY = 505.f;

std::string upperCase(std::string text) {
    for (char& c : text) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return text;
}

sf::Text makeText(const sf::Font& font, const sf::String& value, unsigned size, sf::Color color) {
    sf::Text text(value, font, size);
    text.setFillColor(color);
    return text;
}
} // namespace

GameScene::GameScene(HangmanUI& app, std::unique_ptr<Game> gamePtr)
    : Scene(app),
      game(std::move(gamePtr)),
      word(app.getBoldFont(), 680.f),
      keyboard(app.getBoldFont(), 56.f, 8.f),
      timerBar(app.getBoldFont(), {680.f, 10.f}),
      wordField(app.getFont(), {360.f, 50.f}, 24, utf8("Ghicește tot cuvântul...")),
      guessButton(app.getFont(), utf8("Ghicește"), {140.f, 50.f}, 19),
      hintButton(app.getFont(), utf8("Indiciu"), {170.f, 50.f}, 19),
      menuButton(app.getFont(), utf8("Meniu"), {120.f, 40.f}, 17),
      figurePanel({380.f, 620.f}, 22.f),
      header({1160.f, 60.f}, 16.f),
      resultPanel({640.f, 320.f}, 24.f),
      againButton(app.getBoldFont(), utf8("Joacă din nou"), {196.f, 52.f}, 19),
      resultBoardButton(app.getFont(), utf8("Clasament"), {196.f, 52.f}, 19),
      resultMenuButton(app.getFont(), utf8("Meniu"), {196.f, 52.f}, 19),
      heartPulse(static_cast<std::size_t>(game->getMaxAttempts()), 0.f) {
    const sf::Font& font = app.getFont();
    const sf::Font& bold = app.getBoldFont();

    // ----- antet
    header.setPosition(20.f, 16.f);
    header.setFillColor(withAlpha(theme::Panel, 0.9f));
    header.setOutlineThickness(1.f);
    header.setOutlineColor(theme::Border);
    // Numele nivelului din nucleu este ASCII (pentru consola); in GUI il afisam cu diacritice.
    const std::string level = game->getDifficulty().name == "Usor" ? "Ușor" : game->getDifficulty().name;
    categoryText = makeText(bold, utf8(game->getWordManager().getDisplayName() + "   ·   " + level), 20, theme::Text);
    categoryText.setPosition(46.f, 33.f);
    playerText = makeText(font, utf8("Jucător: " + app.getSettings().playerName), 18, theme::TextMuted);
    centerOrigin(playerText);
    playerText.setPosition(theme::WindowWidth / 2.f, 46.f);
    menuButton.setPosition(1112.f, 46.f);
    menuButton.setOnClick([this] { this->app.showMenu(); });

    // ----- panoul cu spanzuratoarea
    figurePanel.setPosition(40.f, 96.f);
    figurePanel.setFillColor(withAlpha(theme::Panel, 0.75f));
    figurePanel.setOutlineThickness(1.f);
    figurePanel.setOutlineColor(theme::Border);
    figure.setPosition(80.f, 190.f);
    figure.buildGallows(0.15f);
    attemptsLabel = makeText(font, utf8("ÎNCERCĂRI RĂMASE"), 14, theme::TextMuted);
    attemptsLabel.setLetterSpacing(1.6f);
    centerOrigin(attemptsLabel);
    attemptsLabel.setPosition(230.f, 618.f);

    // ----- zona din dreapta
    timerBar.setPosition(RightCenter - 340.f, 128.f);
    word.setPosition(RightCenter, WordY);
    word.setWord(game->getGuessedWord());

    wrongLabel = makeText(font, utf8("GREȘITE"), 14, theme::TextMuted);
    wrongLabel.setLetterSpacing(1.6f);
    wrongLabel.setPosition(RightCenter - 340.f, 302.f);
    wrongLetters = makeText(bold, "", 22, theme::Danger);
    wrongLetters.setLetterSpacing(2.f);
    wrongLetters.setPosition(RightCenter - 240.f, 296.f);

    wordField.setPosition(RightCenter - 160.f, 372.f);
    wordField.setOnSubmit([this](const std::string& text) { guessWord(text); });
    guessButton.setPosition(RightCenter + 100.f, 372.f);
    guessButton.setOnClick([this] { guessWord(wordField.getText()); });
    hintButton.setPalette(theme::GoldDark, sf::Color(146, 102, 14), theme::Gold);
    hintButton.setPosition(RightCenter + 255.f, 372.f);
    hintButton.setOnClick([this] { useHint(); });

    keyboard.setPosition(RightCenter - keyboard.getSize().x / 2.f, 428.f);
    keyboard.setOnLetter([this](char letter) { guessLetter(letter); });

    footer = makeText(font, utf8("Tastează litere   ·   Enter = cuvânt întreg   ·   Tab sau ? = indiciu   ·   Esc = meniu"),
                      14, withAlpha(theme::TextMuted, 0.75f));
    centerOrigin(footer);
    footer.setPosition(RightCenter, 690.f);

    // ----- panoul de final
    resultPanel.setOrigin(resultPanel.getSize() / 2.f);
    resultPanel.setFillColor(theme::Panel);
    resultPanel.setOutlineThickness(2.f);
    resultTitle = makeText(bold, "", 44, theme::Text);
    resultWord = makeText(font, "", 22, theme::Text);
    resultScore = makeText(bold, "", 20, theme::Gold);
    againButton.setPalette(theme::AccentDark, sf::Color(14, 130, 180), theme::Text);
    againButton.setOnClick([this] { this->app.startNewGame(); });
    resultBoardButton.setOnClick([this] { this->app.showLeaderboard(); });
    resultMenuButton.setOnClick([this] { this->app.showMenu(); });

    refreshHud();
}

GameScene::~GameScene() = default;

// ------------------------------------------------------------------ actiuni
void GameScene::guessLetter(char letter) {
    if (roundOver) return;
    letter = static_cast<char>(std::tolower(static_cast<unsigned char>(letter)));

    switch (game->guessLetter(letter)) {
        case GuessResult::Correct: {
            keyboard.markLetter(letter, true);
            word.reveal(game->getGuessedWord(), TileStyle::Revealed);
            const std::string& secret = game->getSelectedWord();
            for (std::size_t i = 0; i < secret.size(); ++i) {
                if (secret[i] == letter) particles.burst(word.getTileCenter(i), 10, 120.f, 280.f);
            }
            break;
        }
        case GuessResult::Wrong:
            keyboard.markLetter(letter, false);
            onWrongGuess();
            break;
        case GuessResult::AlreadyGuessed:
            keyboard.pulseLetter(letter);
            app.notify(utf8("Ai încercat deja litera " + upperCase(std::string(1, letter))), theme::Gold);
            break;
        case GuessResult::Invalid:
        case GuessResult::GameFinished:
            break;
    }
    refreshHud();
    if (game->isGameOver()) finishRound();
}

void GameScene::guessWord(const std::string& text) {
    if (roundOver) return;
    // Camp gol: ignoram. Important: tasta Enter care a activat campul produce
    // imediat si un TextEntered('\r'); daca am pierde focusul aici, literele
    // tastate apoi ar fi interpretate ca incercari individuale.
    if (text.find_first_not_of(' ') == std::string::npos) {
        return;
    }
    wordField.setFocused(false);

    const std::string normalized = WordManager::normalizeWord(text);
    if (normalized.empty()) {
        wordField.shake();
        app.notify(utf8("Folosește doar litere (și eventual spații)"), theme::Gold);
        return;
    }
    if (normalized.size() == 1) {
        wordField.clear();
        guessLetter(normalized[0]);
        return;
    }

    if (game->guessWord(normalized) == GuessResult::Correct) {
        word.reveal(game->getGuessedWord(), TileStyle::Revealed);
    } else {
        wordField.shake();
        onWrongGuess();
        app.notify(utf8("„" + upperCase(normalized) + "” nu este cuvântul căutat"), theme::Danger);
    }
    wordField.clear();
    refreshHud();
    if (game->isGameOver()) finishRound();
}

void GameScene::useHint() {
    if (roundOver) return;
    if (const auto letter = game->useHint()) {
        keyboard.markLetter(*letter, true);
        word.reveal(game->getGuessedWord(), TileStyle::Hint);
        app.notify(utf8("Indiciu: litera " + upperCase(std::string(1, *letter))), theme::Gold);
    } else {
        hintButton.pulse();
        app.notify(utf8("Nu mai ai indicii disponibile"), theme::TextMuted);
    }
    refreshHud();
    if (game->isGameOver()) finishRound();
}

void GameScene::onWrongGuess() {
    figure.setMistakes(game->getMistakes());
    shakeTime = 0.35f;
    redFlash = 1.f;
    const auto lost = static_cast<std::size_t>(std::max(0, game->getAttemptsRemaining()));
    if (lost < heartPulse.size()) heartPulse[lost] = 1.f;
}

void GameScene::refreshHud() {
    const int hints = game->getHintGame().getHintsRemaining();
    hintButton.setLabel(utf8("Indiciu (" + std::to_string(hints) + ")"));
    hintButton.setEnabled(hints > 0 && !roundOver);

    std::string wrong;
    for (char c : game->getWrongLetters()) {
        if (!wrong.empty()) wrong += "  ";
        wrong += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    wrongLetters.setString(wrong.empty() ? utf8("—") : utf8(wrong));
}

void GameScene::finishRound() {
    if (roundOver) return;
    roundOver = true;
    keyboard.setEnabled(false);
    wordField.setEnabled(false);
    guessButton.setEnabled(false);
    hintButton.setEnabled(false);

    // Salvam rezultatul in clasament (si pe disc).
    const std::string& player = app.getSettings().playerName;
    const int score = game->computeScore();
    Leaderboard& board = app.getLeaderboard();
    board.actualizeazaScor(player, score);
    board.salveaza();

    const sf::Color accent = game->isGameWon() ? theme::Success : theme::Danger;
    if (game->isGameWon()) {
        figure.setMood(FigureMood::Won);
        word.celebrate();
        particles.burst({RightCenter, WordY}, 160, 300.f, 760.f);
        particles.setRaining(true);
        confettiTime = 3.f;
        resultTitle.setString(utf8("Victorie!"));
        resultDelay = 1.0f;
    } else {
        figure.setMood(FigureMood::Lost);
        word.revealMissing(game->getSelectedWord());
        shakeTime = 0.5f;
        resultTitle.setString(utf8(game->isTimeUp() ? "Timpul a expirat!" : "Ai fost spânzurat!"));
        resultDelay = 1.5f;
    }
    resultTitle.setFillColor(accent);
    resultPanel.setOutlineColor(accent);
    resultWord.setString(utf8("Cuvântul era:  " + upperCase(game->getSelectedWord())));
    resultScore.setString(utf8((score > 0 ? "+" : "") + std::to_string(score) + " puncte   ·   total " +
                               std::to_string(board.getScorJucator(player))));
    for (sf::Text* text : {&resultTitle, &resultWord, &resultScore}) centerOrigin(*text);
    refreshHud();
}

// ------------------------------------------------------------------ bucla
void GameScene::handleEvent(const sf::Event& event, sf::Vector2f mouse) {
    if (roundOver) {
        if (resultT > 0.6f) {
            againButton.handleEvent(event, mouse);
            resultBoardButton.handleEvent(event, mouse);
            resultMenuButton.handleEvent(event, mouse);
        }
        menuButton.handleEvent(event, mouse);
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter && resultT > 0.6f) app.startNewGame();
            if (event.key.code == sf::Keyboard::Escape) app.showMenu();
        }
        return;
    }

    menuButton.handleEvent(event, mouse);
    if (wordField.handleEvent(event, mouse)) return;
    guessButton.handleEvent(event, mouse);
    hintButton.handleEvent(event, mouse);
    keyboard.handleEvent(event, mouse);

    if (event.type == sf::Event::TextEntered) {
        const sf::Uint32 code = event.text.unicode;
        if (code < 128 && std::isalpha(static_cast<int>(code))) {
            const char letter = static_cast<char>(std::tolower(static_cast<int>(code)));
            keyboard.pulseLetter(letter);
            guessLetter(letter);
        } else if (code == '?') {
            useHint();
        }
    } else if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Enter:  wordField.setFocused(true); break;
            case sf::Keyboard::Tab:    useHint(); break;
            case sf::Keyboard::Escape: app.showMenu(); break;
            default: break;
        }
    }
}

void GameScene::update(float dt) {
    time += dt;
    if (!roundOver) {
        game->update(); // verifica timpul
        if (game->isGameOver()) finishRound();
    }
    const GameTimer& timer = game->getTimer();
    timerBar.setValue(timer.getRemainingFraction(), timer.getTimeRemaining(), timer.hasLimit());

    figure.update(dt);
    word.update(dt);
    keyboard.update(dt);
    timerBar.update(dt);
    wordField.update(dt);
    guessButton.update(dt);
    hintButton.update(dt);
    menuButton.update(dt);
    particles.update(dt);

    if (confettiTime > 0.f) {
        confettiTime -= dt;
        if (confettiTime <= 0.f) particles.setRaining(false);
    }

    // Tremurul ecranului scade liniar in timp.
    if (shakeTime > 0.f) {
        shakeTime = std::max(0.f, shakeTime - dt);
        const float amplitude = 11.f * shakeTime / 0.35f;
        shakeOffset = {std::sin(time * 95.f) * amplitude, std::cos(time * 73.f) * amplitude * 0.6f};
    } else {
        shakeOffset = {0.f, 0.f};
    }
    redFlash = std::max(0.f, redFlash - dt * 3.f);
    for (float& pulse : heartPulse) pulse = std::max(0.f, pulse - dt * 1.8f);

    if (roundOver) {
        if (resultDelay > 0.f) {
            resultDelay -= dt;
        } else {
            resultT = std::min(1.f, resultT + dt / 0.6f);
        }
        const float y = ease::lerp(theme::WindowHeight + 200.f, ResultRestY, ease::outBack(resultT));
        resultPanel.setPosition(RightCenter, y);
        resultTitle.setPosition(RightCenter, y - 100.f);
        resultWord.setPosition(RightCenter, y - 36.f);
        resultScore.setPosition(RightCenter, y + 8.f);
        againButton.setPosition(RightCenter - 206.f, y + 92.f);
        resultBoardButton.setPosition(RightCenter, y + 92.f);
        resultMenuButton.setPosition(RightCenter + 206.f, y + 92.f);
        againButton.update(dt);
        resultBoardButton.update(dt);
        resultMenuButton.update(dt);
    }
}

void GameScene::draw(sf::RenderTarget& target) const {
    sf::RenderStates states;
    states.transform.translate(shakeOffset);

    target.draw(header, states);
    target.draw(categoryText, states);
    target.draw(playerText, states);
    target.draw(menuButton, states);

    target.draw(figurePanel, states);
    target.draw(figure, states);
    target.draw(attemptsLabel, states);

    // inimile: pline = incercari ramase; cea pierduta "explodeaza" scurt
    const std::size_t total = heartPulse.size();
    const auto remaining = static_cast<std::size_t>(std::max(0, game->getAttemptsRemaining()));
    for (std::size_t i = 0; i < total; ++i) {
        sf::Text heart(utf8("♥"), app.getBoldFont(), 30);
        centerOrigin(heart);
        const bool alive = i < remaining;
        const float pulse = heartPulse[i];
        heart.setFillColor(alive ? theme::Danger : ease::lerpColor(theme::Border, theme::Danger, pulse));
        const float s = 1.f + 0.6f * pulse;
        heart.setScale(s, s);
        heart.setPosition(230.f + (static_cast<float>(i) - (static_cast<float>(total) - 1.f) / 2.f) * 38.f,
                          662.f - 10.f * pulse);
        target.draw(heart, states);
    }

    target.draw(timerBar, states);
    target.draw(word, states);
    target.draw(wrongLabel, states);
    target.draw(wrongLetters, states);
    if (resultT < 0.5f) { // sub panoul final, controalele inactive nu mai sunt desenate
        target.draw(wordField, states);
        target.draw(guessButton, states);
        target.draw(hintButton, states);
        target.draw(keyboard, states);
    }
    if (!roundOver) target.draw(footer, states);

    target.draw(particles);

    if (redFlash > 0.f) {
        sf::RectangleShape flash({theme::WindowWidth, theme::WindowHeight});
        flash.setFillColor(withAlpha(theme::Danger, 0.16f * redFlash));
        target.draw(flash);
    }

    if (roundOver && resultT > 0.f) {
        sf::RectangleShape dim({theme::WindowWidth, theme::WindowHeight});
        dim.setFillColor(withAlpha(theme::Background, 0.45f * ease::clamp01(resultT)));
        target.draw(dim);
        RoundedRectangleShape shadow = resultPanel;
        shadow.setFillColor(sf::Color(0, 0, 0, 120));
        shadow.setOutlineThickness(0.f);
        shadow.move(0.f, 10.f);
        target.draw(shadow);
        target.draw(resultPanel);
        target.draw(resultTitle);
        target.draw(resultWord);
        target.draw(resultScore);
        target.draw(againButton);
        target.draw(resultBoardButton);
        target.draw(resultMenuButton);
        target.draw(particles); // confetti si deasupra panoului
    }
}

} // namespace ui
