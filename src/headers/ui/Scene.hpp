//
// Created by Alex on 20.11.2024.
//

#ifndef UI_SCENE_HPP
#define UI_SCENE_HPP

#include <SFML/Graphics.hpp>

class HangmanUI;

namespace ui {

// Un "ecran" al aplicatiei (meniu, joc, clasament). HangmanUI tine o singura
// scena activa printr-un std::unique_ptr<Scene> si ii apeleaza metodele virtuale,
// fara sa stie ce tip concret are (polimorfism la rulare).
class Scene {
public:
    explicit Scene(HangmanUI& app) : app(app) {}
    virtual ~Scene() = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    virtual void handleEvent(const sf::Event& event, sf::Vector2f mouse) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget& target) const = 0;

protected:
    HangmanUI& app; // contextul comun: fonturi, setari, clasament, schimbarea scenei
};

} // namespace ui

#endif //UI_SCENE_HPP
