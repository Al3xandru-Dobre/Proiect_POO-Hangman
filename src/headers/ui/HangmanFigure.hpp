//
// Created by Alex on 20.11.2024.
//

#ifndef UI_HANGMAN_FIGURE_HPP
#define UI_HANGMAN_FIGURE_HPP

#include <array>

#include "Widgets.hpp"

namespace ui {

enum class FigureMood { Playing, Won, Lost };

// Spanzuratoarea desenata procedural (fara imagini): fiecare segment se
// "traseaza" progresiv, iar corpul se balanseaza ca un pendul amortizat.
// Sistem local de coordonate: aproximativ 300 x 400. Pozitia = coltul stanga-sus.
class HangmanFigure : public Widget {
public:
    HangmanFigure();

    void buildGallows(float delay = 0.f);
    void setMistakes(int mistakes);
    void setMood(FigureMood newMood);

    void update(float dt) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    static void drawLine(sf::RenderTarget& target, const sf::RenderStates& states, sf::Vector2f from,
                         sf::Vector2f to, float progress, float thickness, sf::Color color);
    static void drawArc(sf::RenderTarget& target, const sf::RenderStates& states, sf::Vector2f center,
                        float radius, float startDeg, float sweepDeg, float thickness, sf::Color color);
    void drawFace(sf::RenderTarget& target, const sf::RenderStates& states, sf::Color color) const;

    std::array<float, 5> gallows{}; // baza, stalp, grinda, sprijin, franghie
    std::array<float, 6> parts{};   // cap, corp, mana st., mana dr., picior st., picior dr.
    float gallowsDelay = 0.f;
    int targetMistakes = 0;
    FigureMood mood = FigureMood::Playing;
    float moodAmount = 0.f; // 0 -> 1 la schimbarea starii (pentru tranzitii)

    float time = 0.f;
    float swingAngle = 0.f;    // grade
    float swingVelocity = 0.f; // grade / secunda
    float flash = 0.f;         // inrosire scurta la fiecare greseala
};

} // namespace ui

#endif //UI_HANGMAN_FIGURE_HPP
