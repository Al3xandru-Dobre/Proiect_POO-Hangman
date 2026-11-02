//
// Created by Alex on 20.11.2024.
//

#include "headers/ui/Widgets.hpp"

#include <cmath>

#include "headers/ui/Style.hpp"

namespace ui {

// ------------------------------------------------------------------ Widget
void Widget::update(float) {}

bool Widget::handleEvent(const sf::Event&, sf::Vector2f) {
    return false;
}

// ------------------------------------------------------------------ Button
Button::Button(const sf::Font& font, const sf::String& label, sf::Vector2f size, unsigned int characterSize)
    : shape(size, 12.f), shadow(size, 12.f), text(label, font, characterSize), size(size),
      baseColor(theme::PanelLight), hoverColor(theme::Border), textColor(theme::Text) {
    shape.setOrigin(size / 2.f);
    shadow.setOrigin(size / 2.f);
    shadow.setFillColor(sf::Color(0, 0, 0, 90));
    text.setStyle(sf::Text::Bold);
    centerOrigin(text);
}

void Button::setLabel(const sf::String& label) {
    text.setString(label);
    centerOrigin(text);
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = std::move(callback);
}

void Button::setPalette(sf::Color base, sf::Color hover, sf::Color textCol) {
    baseColor = base;
    hoverColor = hover;
    textColor = textCol;
}

void Button::setSelected(bool value) { selected = value; }
void Button::setEnabled(bool value) {
    enabled = value;
    if (!enabled) {
        hovered = pressed = false;
    }
}
void Button::pulse() { pressAmount = 1.f; }
bool Button::isSelected() const { return selected; }
bool Button::isEnabled() const { return enabled; }
sf::Vector2f Button::getSize() const { return size; }

bool Button::contains(sf::Vector2f point) const {
    // Transformam punctul in coordonatele locale ale butonului (tine cont de pozitie/scalare).
    const sf::Vector2f local = getInverseTransform().transformPoint(point);
    return std::abs(local.x) <= size.x / 2.f && std::abs(local.y) <= size.y / 2.f;
}

void Button::onClicked() {
    if (onClick) {
        onClick();
    }
}

bool Button::handleEvent(const sf::Event& event, sf::Vector2f mouse) {
    if (!enabled) {
        return false;
    }
    switch (event.type) {
        case sf::Event::MouseMoved:
            hovered = contains(mouse);
            return false; // hover-ul nu consuma evenimentul
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left && contains(mouse)) {
                pressed = true;
                return true;
            }
            return false;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left && pressed) {
                pressed = false;
                if (contains(mouse)) {
                    pressAmount = 1.f;
                    onClicked();
                }
                return true;
            }
            return false;
        default:
            return false;
    }
}

void Button::update(float dt) {
    hoverAmount = ease::approach(hoverAmount, hovered ? 1.f : 0.f, 14.f, dt);
    selectAmount = ease::approach(selectAmount, selected ? 1.f : 0.f, 12.f, dt);
    enableAmount = ease::approach(enableAmount, enabled ? 1.f : 0.f, 10.f, dt);
    pressAmount = ease::approach(pressAmount, pressed ? 1.f : 0.f, pressed ? 30.f : 9.f, dt);

    visualScale = 1.f + 0.045f * hoverAmount - 0.07f * pressAmount + 0.02f * selectAmount;

    sf::Color fillColor = ease::lerpColor(baseColor, hoverColor, hoverAmount);
    fillColor = ease::lerpColor(fillColor, theme::AccentDark, selectAmount);
    const float alpha = ease::lerp(disabledAlpha, 1.f, enableAmount);
    shape.setFillColor(withAlpha(fillColor, alpha));
    shape.setOutlineThickness(1.5f + 1.f * selectAmount);
    shape.setOutlineColor(withAlpha(ease::lerpColor(theme::Border, theme::Accent, std::max(selectAmount, hoverAmount * 0.6f)), alpha));
    text.setFillColor(withAlpha(textColor, ease::lerp(0.55f, 1.f, enableAmount)));
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    sf::RenderStates shadowStates = states;
    shadowStates.transform.translate(0.f, 4.f - 3.f * pressAmount);
    target.draw(shadow, shadowStates);

    states.transform.scale(visualScale, visualScale);
    target.draw(shape, states);
    target.draw(text, states);
}

// ------------------------------------------------------------------ TextField
TextField::TextField(const sf::Font& font, sf::Vector2f size, std::size_t maxLength, const sf::String& placeholder)
    : box(size, 10.f), content("", font, 22), placeholderText(placeholder, font, 20), caret({2.f, 26.f}),
      size(size), maxLength(maxLength) {
    box.setOrigin(size / 2.f);
    box.setFillColor(theme::Background);
    placeholderText.setFillColor(theme::TextMuted);
    caret.setFillColor(theme::Accent);
    caret.setOrigin(1.f, 13.f);
    refreshText();
}

const std::string& TextField::getText() const { return value; }
void TextField::setText(const std::string& newValue) {
    value = newValue.substr(0, maxLength);
    refreshText();
}
void TextField::clear() { setText(""); }
void TextField::setFocused(bool value) { focused = value && enabled; caretTime = 0.f; }
bool TextField::isFocused() const { return focused; }
void TextField::setEnabled(bool value) {
    enabled = value;
    if (!enabled) focused = false;
}
void TextField::setOnSubmit(std::function<void(const std::string&)> callback) { onSubmit = std::move(callback); }
void TextField::shake() { shakeTime = 0.45f; }

bool TextField::contains(sf::Vector2f point) const {
    const sf::Vector2f local = getInverseTransform().transformPoint(point);
    return std::abs(local.x) <= size.x / 2.f && std::abs(local.y) <= size.y / 2.f;
}

void TextField::refreshText() {
    content.setString(value);
    const sf::FloatRect bounds = content.getLocalBounds();
    content.setOrigin(bounds.left, 0.f);
    content.setPosition(-size.x / 2.f + 16.f, -15.f);
    const sf::FloatRect ph = placeholderText.getLocalBounds();
    placeholderText.setOrigin(ph.left, 0.f);
    placeholderText.setPosition(-size.x / 2.f + 16.f, -13.f);
    caret.setPosition(content.getPosition().x + bounds.width + 3.f, 0.f);
}

bool TextField::handleEvent(const sf::Event& event, sf::Vector2f mouse) {
    if (!enabled) {
        return false;
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        setFocused(contains(mouse));
        return focused;
    }
    if (!focused) {
        return false;
    }
    if (event.type == sf::Event::TextEntered) {
        const sf::Uint32 code = event.text.unicode;
        if (code == 8) { // Backspace
            if (!value.empty()) value.pop_back();
        } else if (code == 13) { // Enter
            if (onSubmit) onSubmit(value);
        } else if (code >= 32 && code < 127 && code != ';' && value.size() < maxLength) {
            value += static_cast<char>(code);
        } else {
            return true;
        }
        caretTime = 0.f;
        refreshText();
        return true;
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            focused = false;
        }
        return true; // cat timp are focus, tastele sunt ale campului
    }
    return false;
}

void TextField::update(float dt) {
    caretTime += dt;
    focusAmount = ease::approach(focusAmount, focused ? 1.f : 0.f, 12.f, dt);
    if (shakeTime > 0.f) {
        shakeTime = std::max(0.f, shakeTime - dt);
        shakeOffset = std::sin(shakeTime * 60.f) * 10.f * (shakeTime / 0.45f);
    } else {
        shakeOffset = 0.f;
    }
    const sf::Color border = ease::lerpColor(theme::Border, theme::Accent, focusAmount);
    box.setOutlineThickness(1.5f + focusAmount);
    box.setOutlineColor(shakeTime > 0.f ? theme::Danger : border);
    box.setFillColor(withAlpha(theme::Background, enabled ? 1.f : 0.5f));
    content.setFillColor(withAlpha(theme::Text, enabled ? 1.f : 0.5f));
}

void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.transform.translate(shakeOffset, 0.f);
    target.draw(box, states);
    if (value.empty() && !focused) {
        target.draw(placeholderText, states);
    } else {
        target.draw(content, states);
    }
    // Cursorul clipeste lin (sinusoidal), nu brusc.
    if (focused) {
        sf::RectangleShape blinking = caret;
        blinking.setFillColor(withAlpha(theme::Accent, 0.5f + 0.5f * std::cos(caretTime * 6.f)));
        target.draw(blinking, states);
    }
}

// ------------------------------------------------------------------ TimerBar
TimerBar::TimerBar(const sf::Font& font, sf::Vector2f size)
    : track(size, size.y / 2.f), fill(size, size.y / 2.f), label("", font, 18), size(size) {
    track.setFillColor(theme::Background);
    track.setOutlineThickness(1.f);
    track.setOutlineColor(theme::Border);
    label.setStyle(sf::Text::Bold);
}

void TimerBar::setValue(float fraction, int secondsLeft, bool hasLimit) {
    targetFraction = fraction;
    seconds = secondsLeft;
    limited = hasLimit;
}

void TimerBar::update(float dt) {
    time += dt;
    shownFraction = ease::approach(shownFraction, targetFraction, 8.f, dt);

    // Verde peste 50%, galben in jur de 25%, rosu spre final.
    const sf::Color color = shownFraction > 0.5f
        ? ease::lerpColor(theme::Gold, theme::Success, (shownFraction - 0.5f) * 2.f)
        : ease::lerpColor(theme::Danger, theme::Gold, shownFraction * 2.f);
    const float width = std::max(size.y, size.x * shownFraction);
    fill.setSize({width, size.y});

    const bool urgent = limited && seconds <= 10 && seconds > 0;
    const float pulse = urgent ? 0.5f + 0.5f * std::sin(time * 10.f) : 1.f;
    fill.setFillColor(limited ? withAlpha(color, 0.65f + 0.35f * pulse) : theme::AccentDark);

    label.setString(limited ? utf8(std::to_string(seconds) + " s") : utf8("∞"));
    label.setFillColor(urgent ? ease::lerpColor(theme::Text, theme::Danger, pulse) : theme::TextMuted);
    const sf::FloatRect b = label.getLocalBounds();
    label.setOrigin(b.left + b.width, b.top + b.height);
    label.setPosition(size.x, -8.f);
    if (urgent) {
        const float s = 1.f + 0.12f * pulse;
        label.setScale(s, s);
    } else {
        label.setScale(1.f, 1.f);
    }
}

void TimerBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(track, states);
    if (shownFraction > 0.002f || !limited) {
        target.draw(fill, states);
    }
    target.draw(label, states);
}

// ------------------------------------------------------------------ ToastStack
ToastStack::ToastStack(const sf::Font& font) : font(font) {}

void ToastStack::push(const sf::String& message, sf::Color accent, float duration) {
    // Nu repetam acelasi mesaj daca este deja afisat (ex. tasta tinuta apasata).
    if (!toasts.empty() && toasts.back().text.getString() == message && toasts.back().age < 1.f) {
        toasts.back().age = 0.3f;
        return;
    }
    Toast toast{sf::Text(message, font, 20), accent, 0.f, duration};
    toast.text.setFillColor(theme::Text);
    centerOrigin(toast.text);
    toasts.push_back(std::move(toast));
    while (toasts.size() > 3) {
        toasts.pop_front();
    }
}

void ToastStack::update(float dt) {
    for (auto& toast : toasts) {
        toast.age += dt;
    }
    while (!toasts.empty() && toasts.front().age >= toasts.front().duration) {
        toasts.pop_front();
    }
}

void ToastStack::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    float y = 0.f;
    for (auto it = toasts.rbegin(); it != toasts.rend(); ++it) {
        const Toast& toast = *it;
        const float enter = ease::outBack(toast.age / 0.35f);
        const float exit = ease::clamp01((toast.duration - toast.age) / 0.4f);
        const float alpha = std::min(ease::clamp01(toast.age / 0.15f), exit);

        const sf::FloatRect bounds = toast.text.getLocalBounds();
        const sf::Vector2f pill{bounds.width + 56.f, 46.f};
        RoundedRectangleShape background(pill, 23.f);
        background.setOrigin(pill / 2.f);
        background.setFillColor(withAlpha(theme::Panel, 0.96f * alpha));
        background.setOutlineThickness(2.f);
        background.setOutlineColor(withAlpha(toast.accent, alpha));

        sf::CircleShape dot(5.f);
        dot.setOrigin(5.f, 5.f);
        dot.setFillColor(withAlpha(toast.accent, alpha));
        dot.setPosition(-pill.x / 2.f + 22.f, 0.f);

        sf::Text text = toast.text;
        text.setFillColor(withAlpha(theme::Text, alpha));
        text.setPosition(8.f, 0.f);

        sf::RenderStates local = states;
        local.transform.translate(0.f, y + ease::lerp(-40.f, 0.f, enter));
        target.draw(background, local);
        target.draw(dot, local);
        target.draw(text, local);
        y += 56.f * alpha;
    }
}

} // namespace ui
