
#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

namespace DrawDebug
{
    void Line(const sf::Vector2f& P1, const sf::Vector2f& P2, const sf::Color& Color, float Thickness = 3.0f, float Duration = -1.0f);

    void LineSegment(const std::vector<sf::Vector2f>& Points, const sf::Color& Color, float Thickness = 3.0f, float Duration = -1.0f);

    void Rect(const sf::Vector2f& Center, const sf::Vector2f& Extent, const sf::Color& Color, float Thickness = 3.0f, float Duration = -1.0f);

    void Circle(const sf::Vector2f& Point, float Radius, const sf::Color& Color, float Thickness = 3.0f, int Segments = 40, float Duration = -1.0f);
}
