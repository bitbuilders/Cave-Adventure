
#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

namespace DrawDebug
{
    void Line(const sf::Vector2f& P1, const sf::Vector2f& P2, sf::Color Color, float Thickness = 3.0f, float Duration = -1.0f);

    void LineSegment(const std::vector<sf::Vector2f>& Points, sf::Color Color, float Thickness = 3.0f, float Duration = -1.0f);
}
