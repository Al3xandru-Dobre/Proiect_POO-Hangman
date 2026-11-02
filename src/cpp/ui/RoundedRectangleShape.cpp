//
// Created by Alex on 20.11.2024.
//

#include "headers/ui/RoundedRectangleShape.hpp"

#include <algorithm>
#include <cmath>

namespace ui {

RoundedRectangleShape::RoundedRectangleShape(sf::Vector2f size, float radius, std::size_t pointsPerCorner)
    : size(size), radius(radius), pointsPerCorner(std::max<std::size_t>(2, pointsPerCorner)) {
    update(); // metoda protejata din sf::Shape: recalculeaza geometria
}

void RoundedRectangleShape::setSize(sf::Vector2f newSize) {
    size = newSize;
    update();
}

sf::Vector2f RoundedRectangleShape::getSize() const {
    return size;
}

void RoundedRectangleShape::setCornerRadius(float newRadius) {
    radius = newRadius;
    update();
}

std::size_t RoundedRectangleShape::getPointCount() const {
    return pointsPerCorner * 4;
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const {
    // Colturile, in sensul acelor de ceas: stanga-sus, dreapta-sus, dreapta-jos, stanga-jos.
    const float r = std::min({radius, size.x / 2.f, size.y / 2.f});
    const std::size_t corner = index / pointsPerCorner;
    const float step = 90.f / static_cast<float>(pointsPerCorner - 1);
    const float degrees = 180.f + 90.f * static_cast<float>(corner) +
                          step * static_cast<float>(index % pointsPerCorner);
    const float radians = degrees * 3.14159265f / 180.f;

    const sf::Vector2f centers[4] = {
        {r, r}, {size.x - r, r}, {size.x - r, size.y - r}, {r, size.y - r}};
    return centers[corner] + sf::Vector2f(std::cos(radians), std::sin(radians)) * r;
}

} // namespace ui
