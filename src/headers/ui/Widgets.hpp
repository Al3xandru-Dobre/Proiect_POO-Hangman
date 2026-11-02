//
// Created by Alex on 20.11.2024.
//

#ifndef UI_WIDGETS_HPP
#define UI_WIDGETS_HPP

#include <SFML/Graphics.hpp>

#include <deque>
#include <functional>
#include <string>

#include "RoundedRectangleShape.hpp"

namespace ui {

// Clasa de baza abstracta pentru toate elementele interfetei.
//  - mosteneste sf::Drawable     -> poate fi desenat cu target.draw(widget)
//  - mosteneste sf::Transformable -> are pozitie, rotatie, scalare
// draw() ramane virtuala pura (din sf::Drawable), deci Widget nu poate fi instantiat.
class Widget : public sf::Drawable, public sf::Transformable {
public:
    ~Widget() override = default;

    // Avanseaza animatiile cu dt secunde. Implicit: nimic de animat.
    virtual void update(float dt);
    // Intoarce true daca evenimentul a fost "consumat" de acest widget.
    virtual bool handleEvent(const sf::Event& event, sf::Vector2f mouse);
};

// Buton cu animatii de hover/apasare/selectie. Pozitia = centrul butonului.
class Button : public Widget {
public:
    Button(const sf::Font& font, const sf::String& label, sf::Vector2f size, unsigned int characterSize = 22);

    void setLabel(const sf::String& label);
    void setOnClick(std::function<void()> callback);
    void setPalette(sf::Color base, sf::Color hover, sf::Color text);
    void setSelected(bool value);
    void setEnabled(bool value);
    void pulse(); // feedback vizual, ca si cum ar fi fost apasat

    [[nodiscard]] bool isSelected() const;
    [[nodiscard]] bool isEnabled() const;
    [[nodiscard]] sf::Vector2f getSize() const;
    [[nodiscard]] bool contains(sf::Vector2f point) const;

    void update(float dt) override;
    bool handleEvent(const sf::Event& event, sf::Vector2f mouse) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual void onClicked(); // punct de extensie pentru clasele derivate

    RoundedRectangleShape shape;
    RoundedRectangleShape shadow;
    sf::Text text;
    sf::Vector2f size;
    sf::Color baseColor;
    sf::Color hoverColor;
    sf::Color textColor;
    float disabledAlpha = 0.4f;

private:
    std::function<void()> onClick;
    bool hovered = false;
    bool pressed = false;
    bool selected = false;
    bool enabled = true;
    float hoverAmount = 0.f;
    float pressAmount = 0.f;
    float selectAmount = 0.f;
    float enableAmount = 1.f;
    float visualScale = 1.f;
};

// Camp de text pe o singura linie (ASCII), cu cursor care clipeste. Pozitia = centrul.
class TextField : public Widget {
public:
    TextField(const sf::Font& font, sf::Vector2f size, std::size_t maxLength, const sf::String& placeholder);

    [[nodiscard]] const std::string& getText() const;
    void setText(const std::string& value);
    void clear();
    void setFocused(bool value);
    [[nodiscard]] bool isFocused() const;
    void setEnabled(bool value);
    void setOnSubmit(std::function<void(const std::string&)> callback);
    void shake(); // animatie de eroare

    void update(float dt) override;
    bool handleEvent(const sf::Event& event, sf::Vector2f mouse) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    [[nodiscard]] bool contains(sf::Vector2f point) const;
    void refreshText();

    RoundedRectangleShape box;
    sf::Text content;
    sf::Text placeholderText;
    sf::RectangleShape caret;
    sf::Vector2f size;
    std::string value;
    std::size_t maxLength;
    std::function<void(const std::string&)> onSubmit;
    bool focused = false;
    bool enabled = true;
    float focusAmount = 0.f;
    float caretTime = 0.f;
    float shakeTime = 0.f;
    float shakeOffset = 0.f;
};

// Bara de timp: se goleste lin si isi schimba culoarea verde -> galben -> rosu.
// Pozitia = coltul stanga-sus.
class TimerBar : public Widget {
public:
    TimerBar(const sf::Font& font, sf::Vector2f size);

    void setValue(float fraction, int secondsLeft, bool hasLimit);
    void update(float dt) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    RoundedRectangleShape track;
    RoundedRectangleShape fill;
    sf::Text label;
    sf::Vector2f size;
    float targetFraction = 1.f;
    float shownFraction = 1.f;
    int seconds = 0;
    bool limited = true;
    float time = 0.f;
};

// Notificari temporare ("toast") care intra de sus si dispar treptat.
// Pozitia = centrul primei notificari.
class ToastStack : public Widget {
public:
    explicit ToastStack(const sf::Font& font);

    void push(const sf::String& message, sf::Color accent, float duration = 2.2f);
    void update(float dt) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    struct Toast {
        sf::Text text;
        sf::Color accent;
        float age = 0.f;
        float duration = 2.f;
    };
    const sf::Font& font;
    std::deque<Toast> toasts;
};

} // namespace ui

#endif //UI_WIDGETS_HPP
