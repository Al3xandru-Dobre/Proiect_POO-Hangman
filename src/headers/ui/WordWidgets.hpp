//
// Created by Alex on 20.11.2024.
//

#ifndef UI_WORD_WIDGETS_HPP
#define UI_WORD_WIDGETS_HPP

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Widgets.hpp"

namespace ui {

enum class TileStyle { Hidden, Revealed, Hint, Missed, Space };

// O casuta pentru o litera din cuvant. Animatii: cadere la inceputul rundei,
// "intoarcere" (flip) la dezvaluire, saritura la victorie. Pozitia = centrul.
class LetterTile : public Widget {
public:
    LetterTile(const sf::Font& font, float size);

    void dropIn(float delay);
    void reveal(char letter, TileStyle style, float delay = 0.f);
    void celebrate(float delay);
    void makeSpace(char symbol); // spatiu sau cratima, vizibile de la inceput
    [[nodiscard]] bool isHidden() const;

    void update(float dt) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void applyStyle(TileStyle visual);

    RoundedRectangleShape box;
    RoundedRectangleShape glowBox;
    sf::Text glyph;
    float size;
    TileStyle style = TileStyle::Hidden;   // starea logica
    TileStyle shown = TileStyle::Hidden;   // starea afisata (se schimba la mijlocul flip-ului)
    char letter = 0;

    float dropDelay = 0.f, dropT = 1.f;
    float flipDelay = 0.f, flipT = 1.f;
    float popT = 1.f;
    float jumpDelay = 0.f, jumpT = 1.f;
    float glow = 0.f;

    // valori calculate in update(), folosite in draw()
    float offsetY = 0.f, scaleX = 1.f, scaleY = 1.f, alpha = 1.f;
};

// Afiseaza tot cuvantul ca sir de LetterTile (compozitie). Pozitia = centrul.
class WordDisplay : public Widget {
public:
    WordDisplay(const sf::Font& font, float maxWidth);

    void setWord(const std::string& progress);
    void reveal(const std::string& progress, TileStyle style);
    void revealMissing(const std::string& secret);
    void celebrate();
    [[nodiscard]] sf::Vector2f getTileCenter(std::size_t index) const;
    [[nodiscard]] std::size_t getLength() const;

    void update(float dt) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    const sf::Font& font;
    float maxWidth;
    float tileSize = 64.f;
    float gap = 10.f;
    std::vector<LetterTile> tiles;
};

enum class KeyState { Unused, Correct, Wrong };

// Tasta virtuala: un Button specializat (mostenire pe 3 niveluri: Widget -> Button -> KeyboardKey).
class KeyboardKey : public Button {
public:
    KeyboardKey(const sf::Font& font, char letter, float size);

    [[nodiscard]] char getLetter() const;
    [[nodiscard]] KeyState getState() const;
    void setState(KeyState newState);

    void update(float dt) override; // extinde Button::update cu animatia de tremur

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    char letter;
    KeyState state = KeyState::Unused;
    float shakeTime = 0.f;
    float shakeOffset = 0.f;
};

// Tastatura QWERTY compusa din 26 de KeyboardKey (pattern-ul Composite).
// Pozitia = coltul stanga-sus.
class OnScreenKeyboard : public Widget {
public:
    OnScreenKeyboard(const sf::Font& font, float keySize, float gap);

    void setOnLetter(std::function<void(char)> callback);
    void markLetter(char letter, bool correct);
    void pulseLetter(char letter);
    void setEnabled(bool value);
    [[nodiscard]] sf::Vector2f getSize() const;

    void update(float dt) override;
    bool handleEvent(const sf::Event& event, sf::Vector2f mouse) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    KeyboardKey* find(char letter);

    std::vector<std::unique_ptr<KeyboardKey>> keys; // adrese stabile pentru callback-uri
    std::function<void(char)> onLetter;
    sf::Vector2f totalSize;
};

} // namespace ui

#endif //UI_WORD_WIDGETS_HPP
