//
// Created by Alex on 20.11.2024.
//

#ifndef UI_ROUNDED_RECTANGLE_SHAPE_HPP
#define UI_ROUNDED_RECTANGLE_SHAPE_HPP

#include <SFML/Graphics/Shape.hpp>

namespace ui {

// SFML nu are dreptunghiuri cu colturi rotunjite, asa ca extindem sf::Shape.
// sf::Shape se ocupa de umplere, contur si texturare; noi trebuie doar sa
// spunem cate puncte are conturul si unde se afla fiecare (2 metode virtuale).
class RoundedRectangleShape : public sf::Shape {
public:
    explicit RoundedRectangleShape(sf::Vector2f size = {0.f, 0.f}, float radius = 0.f,
                                   std::size_t pointsPerCorner = 10);

    void setSize(sf::Vector2f newSize);
    [[nodiscard]] sf::Vector2f getSize() const;
    void setCornerRadius(float newRadius);

    [[nodiscard]] std::size_t getPointCount() const override;
    [[nodiscard]] sf::Vector2f getPoint(std::size_t index) const override;

private:
    sf::Vector2f size;
    float radius;
    std::size_t pointsPerCorner;
};

} // namespace ui

#endif //UI_ROUNDED_RECTANGLE_SHAPE_HPP
