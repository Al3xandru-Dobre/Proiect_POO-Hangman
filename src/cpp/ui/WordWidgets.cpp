//
// Created by Alex on 20.11.2024.
//

#include "headers/ui/WordWidgets.hpp"

#include <array>
#include <cctype>
#include <cmath>

#include "headers/ui/Style.hpp"

namespace ui {

namespace {
constexpr float DropDuration = 0.5f;
constexpr float FlipDuration = 0.38f;
constexpr float PopDuration = 0.28f;
constexpr float JumpDuration = 0.55f;

sf::String upper(char letter) {
    return sf::String(static_cast<char>(std::toupper(static_cast<unsigned char>(letter))));
}
} // namespace

// ------------------------------------------------------------------ LetterTile
LetterTile::LetterTile(const sf::Font& font, float size)
    : box({size, size * 1.15f}, 10.f), glowBox({size + 14.f, size * 1.15f + 14.f}, 16.f),
      glyph("", font, static_cast<unsigned int>(size * 0.58f)), size(size) {
    box.setOrigin(box.getSize() / 2.f);
    glowBox.setOrigin(glowBox.getSize() / 2.f);
    glyph.setStyle(sf::Text::Bold);
    applyStyle(TileStyle::Hidden);
}

void LetterTile::dropIn(float delay) {
    dropDelay = delay;
    dropT = 0.f;
}

void LetterTile::reveal(char newLetter, TileStyle newStyle, float delay) {
    if (style == TileStyle::Space) return;
    letter = newLetter;
    style = newStyle;
    flipDelay = delay;
    flipT = 0.f;
}

void LetterTile::celebrate(float delay) {
    if (style == TileStyle::Space) return;
    jumpDelay = delay;
    jumpT = 0.f;
}

void LetterTile::makeSpace(char symbol) {
    style = shown = TileStyle::Space;
    letter = symbol;
    glyph.setString(symbol == '-' ? sf::String("-") : sf::String(""));
    centerOrigin(glyph);
}

bool LetterTile::isHidden() const {
    return style == TileStyle::Hidden;
}

void LetterTile::applyStyle(TileStyle visual) {
    shown = visual;
    switch (visual) {
        case TileStyle::Hidden:
            box.setFillColor(theme::Panel);
            box.setOutlineColor(theme::Border);
            glyph.setString("");
            break;
        case TileStyle::Revealed:
            box.setFillColor(theme::PanelLight);
            box.setOutlineColor(theme::Accent);
            glyph.setFillColor(theme::Text);
            break;
        case TileStyle::Hint:
            box.setFillColor(theme::GoldDark);
            box.setOutlineColor(theme::Gold);
            glyph.setFillColor(theme::Gold);
            break;
        case TileStyle::Missed:
            box.setFillColor(theme::DangerDark);
            box.setOutlineColor(theme::Danger);
            glyph.setFillColor(theme::Danger);
            break;
        case TileStyle::Space:
            break;
    }
    box.setOutlineThickness(2.f);
    if (visual != TileStyle::Hidden && visual != TileStyle::Space) {
        glyph.setString(upper(letter));
        centerOrigin(glyph);
    }
}

void LetterTile::update(float dt) {
    // 1. Caderea de la inceputul rundei (cu "arc" la aterizare).
    if (dropDelay > 0.f) dropDelay -= dt;
    else dropT = std::min(1.f, dropT + dt / DropDuration);

    // 2. Flip: scalarea pe Y scade la 0, schimbam fata, apoi creste inapoi la 1.
    if (flipT < 1.f) {
        if (flipDelay > 0.f) {
            flipDelay -= dt;
        } else {
            const float before = flipT;
            flipT = std::min(1.f, flipT + dt / FlipDuration);
            if (before < 0.5f && flipT >= 0.5f) {
                applyStyle(style);
                glow = 1.f;
            }
            if (flipT >= 1.f) popT = 0.f;
        }
    }
    if (popT < 1.f) popT = std::min(1.f, popT + dt / PopDuration);

    // 3. Saritura de victorie.
    if (jumpT < 1.f) {
        if (jumpDelay > 0.f) jumpDelay -= dt;
        else jumpT = std::min(1.f, jumpT + dt / JumpDuration);
    }
    glow = std::max(0.f, glow - dt * 1.6f);

    const float drop = ease::outBack(dropT);
    offsetY = ease::lerp(-70.f, 0.f, drop) - std::sin(ease::Pi * jumpT) * 26.f * (jumpT < 1.f ? 1.f : 0.f);
    alpha = ease::clamp01(dropT * 2.5f);
    const float pop = 1.f + 0.14f * std::sin(ease::Pi * popT);
    scaleX = pop;
    scaleY = (flipT < 1.f ? std::abs(std::cos(ease::Pi * flipT)) : 1.f) * pop;
}

void LetterTile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.transform.translate(0.f, offsetY).scale(scaleX, scaleY);

    if (shown == TileStyle::Space) {
        sf::Text dash = glyph;
        dash.setFillColor(withAlpha(theme::TextMuted, alpha));
        target.draw(dash, states);
        return;
    }

    if (glow > 0.f) {
        RoundedRectangleShape halo = glowBox;
        halo.setFillColor(withAlpha(box.getOutlineColor(), 0.45f * glow * alpha));
        target.draw(halo, states);
    }
    RoundedRectangleShape body = box;
    body.setFillColor(withAlpha(box.getFillColor(), alpha));
    body.setOutlineColor(withAlpha(box.getOutlineColor(), alpha));
    target.draw(body, states);

    if (shown == TileStyle::Hidden) {
        // linie subtire, ca un "_" stilizat
        sf::RectangleShape underline({size * 0.42f, 3.f});
        underline.setOrigin(size * 0.21f, 1.5f);
        underline.setPosition(0.f, size * 0.32f);
        underline.setFillColor(withAlpha(theme::Border, alpha));
        target.draw(underline, states);
    } else {
        sf::Text text = glyph;
        text.setFillColor(withAlpha(glyph.getFillColor(), alpha));
        target.draw(text, states);
    }
}

// ------------------------------------------------------------------ WordDisplay
WordDisplay::WordDisplay(const sf::Font& font, float maxWidth) : font(font), maxWidth(maxWidth) {}

void WordDisplay::setWord(const std::string& progress) {
    tiles.clear();
    const float count = static_cast<float>(progress.size());
    tileSize = std::min(64.f, (maxWidth - gap * (count - 1.f)) / count);
    gap = std::min(10.f, tileSize * 0.16f);

    const float total = count * tileSize + (count - 1.f) * gap;
    for (std::size_t i = 0; i < progress.size(); ++i) {
        LetterTile tile(font, tileSize);
        tile.setPosition(-total / 2.f + tileSize / 2.f + static_cast<float>(i) * (tileSize + gap), 0.f);
        if (progress[i] != '_') {
            tile.makeSpace(progress[i]);
        }
        tile.dropIn(0.05f * static_cast<float>(i)); // efect "val", litera cu litera
        tiles.push_back(std::move(tile));
    }
}

void WordDisplay::reveal(const std::string& progress, TileStyle style) {
    float delay = 0.f;
    for (std::size_t i = 0; i < tiles.size() && i < progress.size(); ++i) {
        if (tiles[i].isHidden() && progress[i] != '_') {
            tiles[i].reveal(progress[i], style, delay);
            delay += 0.08f;
        }
    }
}

void WordDisplay::revealMissing(const std::string& secret) {
    float delay = 0.25f;
    for (std::size_t i = 0; i < tiles.size() && i < secret.size(); ++i) {
        if (tiles[i].isHidden()) {
            tiles[i].reveal(secret[i], TileStyle::Missed, delay);
            delay += 0.11f;
        }
    }
}

void WordDisplay::celebrate() {
    for (std::size_t i = 0; i < tiles.size(); ++i) {
        tiles[i].celebrate(0.35f + 0.06f * static_cast<float>(i));
    }
}

sf::Vector2f WordDisplay::getTileCenter(std::size_t index) const {
    if (index >= tiles.size()) return getPosition();
    return getTransform().transformPoint(tiles[index].getPosition());
}

std::size_t WordDisplay::getLength() const {
    return tiles.size();
}

void WordDisplay::update(float dt) {
    for (auto& tile : tiles) tile.update(dt);
}

void WordDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    for (const auto& tile : tiles) target.draw(tile, states);
}

// ------------------------------------------------------------------ KeyboardKey
KeyboardKey::KeyboardKey(const sf::Font& font, char letter, float size)
    : Button(font, upper(letter), {size, size * 1.08f}, static_cast<unsigned int>(size * 0.42f)), letter(letter) {
    setPalette(theme::PanelLight, theme::Border, theme::Text);
}

char KeyboardKey::getLetter() const { return letter; }
KeyState KeyboardKey::getState() const { return state; }

void KeyboardKey::setState(KeyState newState) {
    state = newState;
    disabledAlpha = 0.9f; // tastele folosite raman vizibile, doar colorate
    switch (state) {
        case KeyState::Unused:
            setPalette(theme::PanelLight, theme::Border, theme::Text);
            setEnabled(true);
            break;
        case KeyState::Correct:
            setPalette(theme::SuccessDark, theme::SuccessDark, theme::Success);
            setEnabled(false);
            pulse();
            break;
        case KeyState::Wrong:
            setPalette(theme::DangerDark, theme::DangerDark, theme::Danger);
            setEnabled(false);
            shakeTime = 0.4f;
            break;
    }
}

void KeyboardKey::update(float dt) {
    Button::update(dt); // reutilizam comportamentul clasei de baza...
    if (shakeTime > 0.f) { // ...si adaugam doar ce este specific tastei
        shakeTime = std::max(0.f, shakeTime - dt);
        shakeOffset = std::sin(shakeTime * 70.f) * 6.f * (shakeTime / 0.4f);
    } else {
        shakeOffset = 0.f;
    }
}

void KeyboardKey::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform.translate(shakeOffset, 0.f);
    Button::draw(target, states);
}

// ------------------------------------------------------------------ OnScreenKeyboard
OnScreenKeyboard::OnScreenKeyboard(const sf::Font& font, float keySize, float gap) {
    const std::array<std::string, 3> rows = {"qwertyuiop", "asdfghjkl", "zxcvbnm"};
    const float keyHeight = keySize * 1.08f;
    const float widest = static_cast<float>(rows[0].size()) * (keySize + gap) - gap;

    for (std::size_t r = 0; r < rows.size(); ++r) {
        const float rowWidth = static_cast<float>(rows[r].size()) * (keySize + gap) - gap;
        const float startX = (widest - rowWidth) / 2.f + keySize / 2.f;
        for (std::size_t c = 0; c < rows[r].size(); ++c) {
            const char letter = rows[r][c];
            auto key = std::make_unique<KeyboardKey>(font, letter, keySize);
            key->setPosition(startX + static_cast<float>(c) * (keySize + gap),
                             keyHeight / 2.f + static_cast<float>(r) * (keyHeight + gap));
            key->setOnClick([this, letter] {
                if (onLetter) onLetter(letter);
            });
            keys.push_back(std::move(key));
        }
    }
    totalSize = {widest, 3.f * keyHeight + 2.f * gap};
}

void OnScreenKeyboard::setOnLetter(std::function<void(char)> callback) { onLetter = std::move(callback); }

KeyboardKey* OnScreenKeyboard::find(char letter) {
    for (auto& key : keys) {
        if (key->getLetter() == letter) return key.get();
    }
    return nullptr;
}

void OnScreenKeyboard::markLetter(char letter, bool correct) {
    if (KeyboardKey* key = find(letter)) key->setState(correct ? KeyState::Correct : KeyState::Wrong);
}

void OnScreenKeyboard::pulseLetter(char letter) {
    if (KeyboardKey* key = find(letter)) key->pulse();
}

void OnScreenKeyboard::setEnabled(bool value) {
    for (auto& key : keys) {
        if (key->getState() == KeyState::Unused) key->setEnabled(value);
    }
}

sf::Vector2f OnScreenKeyboard::getSize() const { return totalSize; }

void OnScreenKeyboard::update(float dt) {
    for (auto& key : keys) key->update(dt);
}

bool OnScreenKeyboard::handleEvent(const sf::Event& event, sf::Vector2f mouse) {
    // Mouse-ul este convertit in coordonatele locale ale tastaturii,
    // apoi evenimentul este delegat fiecarei taste.
    const sf::Vector2f local = getInverseTransform().transformPoint(mouse);
    bool consumed = false;
    for (auto& key : keys) {
        consumed = key->handleEvent(event, local) || consumed;
    }
    return consumed;
}

void OnScreenKeyboard::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    for (const auto& key : keys) target.draw(*key, states);
}

} // namespace ui
