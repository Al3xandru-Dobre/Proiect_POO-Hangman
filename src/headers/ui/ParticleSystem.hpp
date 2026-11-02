//
// Created by Alex on 20.11.2024.
//

#ifndef UI_PARTICLE_SYSTEM_HPP
#define UI_PARTICLE_SYSTEM_HPP

#include <vector>

#include "Widgets.hpp"

namespace ui {

// Confetti: sute de dreptunghiuri mici, simulate cu gravitatie si frecare,
// desenate intr-un singur apel (sf::VertexArray) pentru performanta.
class ParticleSystem : public Widget {
public:
    void burst(sf::Vector2f origin, int count, float minSpeed, float maxSpeed);
    void setRaining(bool value, float width = 1200.f);
    void clear();

    void update(float dt) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    struct Particle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Color color;
        float life;
        float maxLife;
        float angle;
        float spin;
        sf::Vector2f size;
    };
    void spawn(sf::Vector2f origin, sf::Vector2f velocity);

    std::vector<Particle> particles;
    bool raining = false;
    float rainWidth = 1200.f;
    float rainAccumulator = 0.f;
};

} // namespace ui

#endif //UI_PARTICLE_SYSTEM_HPP
