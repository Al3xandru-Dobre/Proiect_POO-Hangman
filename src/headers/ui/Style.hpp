//
// Created by Alex on 20.11.2024.
//

#ifndef UI_STYLE_HPP
#define UI_STYLE_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <cmath>
#include <string_view>

namespace ui {

// Paleta de culori a interfetei (tema inchisa, accente reci).
namespace theme {
inline const sf::Color Background{11, 17, 32};
inline const sf::Color BackgroundTop{24, 36, 62};
inline const sf::Color Panel{22, 32, 52};
inline const sf::Color PanelLight{38, 52, 78};
inline const sf::Color Border{61, 78, 108};
inline const sf::Color Text{238, 242, 248};
inline const sf::Color TextMuted{140, 154, 180};
inline const sf::Color Accent{56, 189, 248};
inline const sf::Color AccentDark{12, 98, 140};
inline const sf::Color Success{74, 222, 128};
inline const sf::Color SuccessDark{22, 101, 52};
inline const sf::Color Danger{248, 113, 113};
inline const sf::Color DangerDark{127, 29, 29};
inline const sf::Color Gold{251, 191, 36};
inline const sf::Color GoldDark{120, 83, 10};
inline const sf::Color Wood{176, 128, 86};
inline const sf::Color Rope{214, 186, 140};

inline constexpr float WindowWidth = 1200.f;
inline constexpr float WindowHeight = 760.f;
} // namespace theme

// Textele din cod sunt UTF-8 (diacritice); SFML lucreaza intern cu UTF-32.
inline sf::String utf8(std::string_view text) {
    return sf::String::fromUtf8(text.begin(), text.end());
}

inline sf::Color withAlpha(sf::Color color, float alpha) {
    color.a = static_cast<sf::Uint8>(std::clamp(alpha, 0.f, 1.f) * 255.f);
    return color;
}

// Aseaza originea textului in centrul sau vizual (pentru centrare si scalare).
// Pe verticala centram dupa inaltimea majusculei "H", nu dupa conturul textului:
// altfel diacriticele (Ș, Ă, Î) sau literele cu coada (g, p) ar muta eticheta.
inline void centerOrigin(sf::Text& text) {
    const sf::FloatRect bounds = text.getLocalBounds();
    float centerY = bounds.top + bounds.height / 2.f;
    if (const sf::Font* font = text.getFont()) {
        const sf::Glyph& reference = font->getGlyph(U'H', text.getCharacterSize(), text.getStyle() & sf::Text::Bold);
        centerY = reference.bounds.top + reference.bounds.height / 2.f + static_cast<float>(text.getCharacterSize());
    }
    text.setOrigin(bounds.left + bounds.width / 2.f, centerY);
}

// Functii de "easing": transforma progresul liniar t in [0,1] intr-o miscare
// naturala (accelerare, decelerare, depasire usoara a tintei etc.).
namespace ease {
inline constexpr float Pi = 3.14159265358979f;

inline float clamp01(float t) { return std::clamp(t, 0.f, 1.f); }
inline float lerp(float a, float b, float t) { return a + (b - a) * t; }
inline sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t) { return a + (b - a) * t; }

inline sf::Color lerpColor(sf::Color a, sf::Color b, float t) {
    t = clamp01(t);
    const auto mix = [t](sf::Uint8 x, sf::Uint8 y) {
        return static_cast<sf::Uint8>(std::lround(lerp(static_cast<float>(x), static_cast<float>(y), t)));
    };
    return {mix(a.r, b.r), mix(a.g, b.g), mix(a.b, b.b), mix(a.a, b.a)};
}

inline float outCubic(float t) { t = clamp01(t); return 1.f - std::pow(1.f - t, 3.f); }
inline float inOutCubic(float t) {
    t = clamp01(t);
    return t < 0.5f ? 4.f * t * t * t : 1.f - std::pow(-2.f * t + 2.f, 3.f) / 2.f;
}
// Depaseste putin tinta si revine (efect de "arc").
inline float outBack(float t) {
    t = clamp01(t);
    const float c1 = 1.70158f, c3 = c1 + 1.f;
    return 1.f + c3 * std::pow(t - 1.f, 3.f) + c1 * std::pow(t - 1.f, 2.f);
}
inline float outElastic(float t) {
    t = clamp01(t);
    if (t == 0.f || t == 1.f) return t;
    return std::pow(2.f, -10.f * t) * std::sin((t * 10.f - 0.75f) * (2.f * Pi / 3.f)) + 1.f;
}
// Apropiere exponentiala de o tinta, independenta de numarul de cadre pe secunda.
inline float approach(float current, float target, float speed, float dt) {
    return current + (target - current) * (1.f - std::exp(-speed * dt));
}
} // namespace ease

} // namespace ui

#endif //UI_STYLE_HPP
