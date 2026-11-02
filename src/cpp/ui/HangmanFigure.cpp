//
// Created by Alex on 20.11.2024.
//

#include "headers/ui/HangmanFigure.hpp"

#include <cmath>

#include "headers/ui/Style.hpp"

namespace ui {

namespace {
constexpr float SegmentDuration = 0.28f;
constexpr float PartDuration = 0.42f;
const sf::Vector2f Pivot{200.f, 30.f}; // punctul de prindere al franghiei

// Geometria (coordonate locale).
const sf::Vector2f HeadCenter{200.f, 122.f};
constexpr float HeadRadius = 30.f;
const sf::Vector2f Neck{200.f, 152.f}, Hip{200.f, 252.f}, Shoulder{200.f, 178.f};
const sf::Vector2f LeftHandDown{158.f, 226.f}, RightHandDown{242.f, 226.f};
const sf::Vector2f LeftHandUp{160.f, 132.f}, RightHandUp{240.f, 132.f};
const sf::Vector2f LeftFoot{166.f, 322.f}, RightFoot{234.f, 322.f};
} // namespace

HangmanFigure::HangmanFigure() = default;

void HangmanFigure::buildGallows(float delay) {
    gallows.fill(0.f);
    gallowsDelay = delay;
}

void HangmanFigure::setMistakes(int mistakes) {
    if (mistakes > targetMistakes) {
        flash = 1.f;
        swingVelocity += (mistakes % 2 == 0 ? 1.f : -1.f) * 55.f; // impuls lateral
    }
    targetMistakes = std::clamp(mistakes, 0, static_cast<int>(parts.size()));
}

void HangmanFigure::setMood(FigureMood newMood) {
    if (newMood == mood) return;
    mood = newMood;
    moodAmount = 0.f;
    if (mood == FigureMood::Lost) {
        swingVelocity += 140.f; // impuls puternic: corpul incepe sa se legene
        for (auto& p : parts) p = std::max(p, 0.999f);
    }
}

void HangmanFigure::update(float dt) {
    time += dt;
    moodAmount = std::min(1.f, moodAmount + dt / 0.6f);
    flash = std::max(0.f, flash - dt * 2.2f);

    // Segmentele se deseneaza in ordine: urmatorul porneste doar dupa ce precedentul s-a terminat.
    if (gallowsDelay > 0.f) {
        gallowsDelay -= dt;
    } else {
        for (auto& segment : gallows) {
            if (segment < 1.f) {
                segment = std::min(1.f, segment + dt / SegmentDuration);
                break;
            }
        }
    }
    const bool gallowsReady = gallows.back() >= 1.f;
    for (int i = 0; i < targetMistakes && gallowsReady; ++i) {
        auto& part = parts[static_cast<std::size_t>(i)];
        if (part < 1.f) {
            part = std::min(1.f, part + dt / PartDuration);
            break;
        }
    }

    // Pendul amortizat: acceleratie = -k * unghi - c * viteza (+ o mica forta de "respiratie").
    const float stiffness = 14.f;
    const float damping = mood == FigureMood::Lost ? 0.18f : 2.4f;
    const float idle = mood == FigureMood::Lost ? 0.f : std::sin(time * 1.7f) * 6.f;
    const float acceleration = -stiffness * swingAngle - damping * swingVelocity + idle;
    swingVelocity += acceleration * dt;
    swingAngle += swingVelocity * dt;
}

void HangmanFigure::drawLine(sf::RenderTarget& target, const sf::RenderStates& states, sf::Vector2f from,
                             sf::Vector2f to, float progress, float thickness, sf::Color color) {
    if (progress <= 0.f) return;
    const sf::Vector2f end = ease::lerp(from, to, ease::outCubic(progress));
    const sf::Vector2f delta = end - from;
    const float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    sf::RectangleShape line({length, thickness});
    line.setOrigin(0.f, thickness / 2.f);
    line.setPosition(from);
    line.setRotation(std::atan2(delta.y, delta.x) * 180.f / ease::Pi);
    line.setFillColor(color);
    target.draw(line, states);

    // capete rotunjite
    sf::CircleShape cap(thickness / 2.f, 16);
    cap.setOrigin(thickness / 2.f, thickness / 2.f);
    cap.setFillColor(color);
    cap.setPosition(from);
    target.draw(cap, states);
    cap.setPosition(end);
    target.draw(cap, states);
}

void HangmanFigure::drawArc(sf::RenderTarget& target, const sf::RenderStates& states, sf::Vector2f center,
                            float radius, float startDeg, float sweepDeg, float thickness, sf::Color color) {
    if (std::abs(sweepDeg) < 0.5f) return;
    const int segments = std::max(4, static_cast<int>(std::abs(sweepDeg) / 6.f));
    sf::VertexArray strip(sf::TriangleStrip);
    for (int i = 0; i <= segments; ++i) {
        const float angle = (startDeg + sweepDeg * static_cast<float>(i) / static_cast<float>(segments)) * ease::Pi / 180.f;
        const sf::Vector2f direction{std::cos(angle), std::sin(angle)};
        strip.append(sf::Vertex(center + direction * (radius - thickness / 2.f), color));
        strip.append(sf::Vertex(center + direction * (radius + thickness / 2.f), color));
    }
    target.draw(strip, states);
}

void HangmanFigure::drawFace(sf::RenderTarget& target, const sf::RenderStates& states, sf::Color color) const {
    const float appear = ease::clamp01((parts[0] - 0.85f) / 0.15f);
    if (appear <= 0.f) return;
    const sf::Color faceColor = withAlpha(color, appear);
    const sf::Vector2f leftEye = HeadCenter + sf::Vector2f(-11.f, -6.f);
    const sf::Vector2f rightEye = HeadCenter + sf::Vector2f(11.f, -6.f);

    if (mood == FigureMood::Lost) {
        // ochi in forma de X
        for (const auto& eye : {leftEye, rightEye}) {
            drawLine(target, states, eye + sf::Vector2f(-5.f, -5.f), eye + sf::Vector2f(5.f, 5.f), moodAmount, 3.f, faceColor);
            drawLine(target, states, eye + sf::Vector2f(5.f, -5.f), eye + sf::Vector2f(-5.f, 5.f), moodAmount, 3.f, faceColor);
        }
        drawArc(target, states, HeadCenter + sf::Vector2f(0.f, 20.f), 9.f, 200.f, 140.f, 3.f, faceColor);
        return;
    }

    // ochi care clipesc din cand in cand
    const bool blink = std::fmod(time, 3.7f) < 0.12f;
    sf::CircleShape eye(3.5f);
    eye.setOrigin(3.5f, 3.5f);
    eye.setFillColor(faceColor);
    eye.setScale(1.f, blink ? 0.15f : 1.f);
    eye.setPosition(leftEye);
    target.draw(eye, states);
    eye.setPosition(rightEye);
    target.draw(eye, states);

    if (mood == FigureMood::Won) {
        drawArc(target, states, HeadCenter + sf::Vector2f(0.f, 2.f), 15.f, 20.f, 140.f * moodAmount, 3.f, faceColor);
    } else {
        // gura devine tot mai ingrijorata pe masura ce cresc greselile
        const float worry = static_cast<float>(targetMistakes) / 6.f;
        drawLine(target, states, HeadCenter + sf::Vector2f(-8.f, 13.f + 2.f * worry),
                 HeadCenter + sf::Vector2f(8.f, 13.f - 2.f * worry), 1.f, 3.f, faceColor);
    }
}

void HangmanFigure::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    // Spanzuratoarea (fixa)
    const sf::Color wood = theme::Wood;
    drawLine(target, states, {20.f, 380.f}, {280.f, 380.f}, gallows[0], 10.f, wood);
    drawLine(target, states, {70.f, 380.f}, {70.f, 30.f}, gallows[1], 10.f, wood);
    drawLine(target, states, {64.f, 30.f}, {206.f, 30.f}, gallows[2], 10.f, wood);
    drawLine(target, states, {70.f, 92.f}, {130.f, 32.f}, gallows[3], 7.f, wood);

    // Tot ce atarna de franghie se roteste in jurul punctului Pivot.
    sf::RenderStates body = states;
    body.transform.rotate(swingAngle, Pivot);
    drawLine(target, body, Pivot, {200.f, 92.f}, gallows[4], 4.f, theme::Rope);

    sf::Color color = mood == FigureMood::Lost ? ease::lerpColor(theme::Text, theme::Danger, moodAmount * 0.7f)
                                               : theme::Text;
    color = ease::lerpColor(color, theme::Danger, flash);

    // Pe victorie, corpul "topaie" usor si mainile se ridica.
    if (mood == FigureMood::Won) {
        body.transform.translate(0.f, -std::abs(std::sin(time * 5.f)) * 8.f * moodAmount);
    }
    const float raise = mood == FigureMood::Won ? ease::outBack(moodAmount) : 0.f;
    const float wave = mood == FigureMood::Won ? std::sin(time * 9.f) * 8.f * moodAmount : 0.f;
    const sf::Vector2f leftHand = ease::lerp(LeftHandDown, LeftHandUp + sf::Vector2f(-wave, 0.f), raise);
    const sf::Vector2f rightHand = ease::lerp(RightHandDown, RightHandUp + sf::Vector2f(wave, 0.f), raise);

    drawArc(target, body, HeadCenter, HeadRadius, -90.f, 360.f * ease::outCubic(parts[0]), 6.f, color);
    drawFace(target, body, color);
    drawLine(target, body, Neck, Hip, parts[1], 6.f, color);
    drawLine(target, body, Shoulder, leftHand, parts[2], 6.f, color);
    drawLine(target, body, Shoulder, rightHand, parts[3], 6.f, color);
    drawLine(target, body, Hip, LeftFoot, parts[4], 6.f, color);
    drawLine(target, body, Hip, RightFoot, parts[5], 6.f, color);
}

} // namespace ui
