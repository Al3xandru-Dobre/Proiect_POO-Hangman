//
// Created by Alex on 20.11.2024.
//

#include "headers/ui/ParticleSystem.hpp"

#include <algorithm>
#include <array>
#include <cmath>

#include "headers/Random.hpp"
#include "headers/ui/Style.hpp"

namespace ui {

namespace {
const std::array<sf::Color, 6> Palette = {theme::Accent, theme::Success, theme::Gold,
                                          theme::Danger, sf::Color(167, 139, 250), sf::Color(244, 114, 182)};
}

void ParticleSystem::spawn(sf::Vector2f origin, sf::Vector2f velocity) {
    Particle p;
    p.position = origin;
    p.velocity = velocity;
    p.color = Palette[static_cast<std::size_t>(Random::between(0, static_cast<int>(Palette.size()) - 1))];
    p.maxLife = p.life = Random::realBetween(1.6f, 3.0f);
    p.angle = Random::realBetween(0.f, 360.f);
    p.spin = Random::realBetween(-540.f, 540.f);
    p.size = {Random::realBetween(6.f, 11.f), Random::realBetween(3.f, 6.f)};
    particles.push_back(p);
}

void ParticleSystem::burst(sf::Vector2f origin, int count, float minSpeed, float maxSpeed) {
    for (int i = 0; i < count; ++i) {
        const float angle = Random::realBetween(-ease::Pi, 0.f); // in sus, in evantai
        const float speed = Random::realBetween(minSpeed, maxSpeed);
        spawn(origin, {std::cos(angle) * speed, std::sin(angle) * speed});
    }
}

void ParticleSystem::setRaining(bool value, float width) {
    raining = value;
    rainWidth = width;
}

void ParticleSystem::clear() {
    particles.clear();
    raining = false;
}

void ParticleSystem::update(float dt) {
    if (raining) {
        rainAccumulator += dt * 70.f; // ~70 particule pe secunda
        while (rainAccumulator >= 1.f) {
            rainAccumulator -= 1.f;
            spawn({Random::realBetween(0.f, rainWidth), -20.f},
                  {Random::realBetween(-40.f, 40.f), Random::realBetween(60.f, 160.f)});
        }
    }

    constexpr float Gravity = 520.f;
    for (auto& p : particles) {
        p.velocity.y += Gravity * dt;
        p.velocity *= std::pow(0.35f, dt); // rezistenta aerului
        p.velocity.x += std::sin(p.angle * 0.05f) * 30.f * dt; // usoara fluturare
        p.position += p.velocity * dt;
        p.angle += p.spin * dt;
        p.life -= dt;
    }
    // Idiomul erase-remove: stergem particulele expirate intr-o singura trecere.
    particles.erase(std::remove_if(particles.begin(), particles.end(),
                                   [](const Particle& p) { return p.life <= 0.f || p.position.y > 900.f; }),
                    particles.end());
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (particles.empty()) return;
    states.transform *= getTransform();

    sf::VertexArray quads(sf::Triangles);
    for (const auto& p : particles) {
        const float radians = p.angle * ease::Pi / 180.f;
        // "flip" 3D fals: latimea oscileaza ca la o hartie care se roteste
        const sf::Vector2f half{p.size.x / 2.f * std::cos(radians * 2.f), p.size.y / 2.f};
        const float c = std::cos(radians), s = std::sin(radians);
        const auto corner = [&](float x, float y) {
            return p.position + sf::Vector2f(x * c - y * s, x * s + y * c);
        };
        const sf::Color color = withAlpha(p.color, std::min(1.f, p.life / 0.6f));
        const sf::Vector2f a = corner(-half.x, -half.y), b = corner(half.x, -half.y);
        const sf::Vector2f d = corner(half.x, half.y), e = corner(-half.x, half.y);
        quads.append({a, color});
        quads.append({b, color});
        quads.append({d, color});
        quads.append({a, color});
        quads.append({d, color});
        quads.append({e, color});
    }
    target.draw(quads, states);
}

} // namespace ui
