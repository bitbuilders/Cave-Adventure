
#include "DebugDraw.h"

#include "Console.h"
#include "Game.h"

void DrawDebug::Line(const sf::Vector2f& P1, const sf::Vector2f& P2, sf::Color Color, float Thickness, float Duration)
{
    LineSegment({P1, P2}, Color, Thickness, Duration);
}

void DrawDebug::LineSegment(const std::vector<sf::Vector2f>& Points, sf::Color Color, float Thickness, float Duration)
{
    if (Points.size() < 2)
    {
        LOG_WARN("Tried to make a line with less than 2 points!");
        return;
    }

    sf::VertexArray lines{ sf::PrimitiveType::TriangleStrip, Points.size() * 2 };

    for (int i = 0; i < Points.size(); ++i)
    {
        int lineIndex = i * 2;
        lines[lineIndex].color = Color;
        lines[lineIndex + 1].color = Color;

        sf::Vector2f point = Points[i];

        sf::Vector2f dir;
        if (i < Points.size() - 1)
        {
            sf::Vector2f target = Points[i + 1];
            dir = target - point;
        }
        else
        {
            sf::Vector2f target = Points[i - 1];
            dir = point - target;
        }

        sf::Transform rotation;
        rotation.rotate(sf::degrees(90.0f));

        sf::Vector2f p1 = rotation.transformPoint(dir);
        sf::Vector2f p2 = -p1;

        lines[lineIndex].position = p1.normalized() * Thickness + point;
        lines[lineIndex + 1].position = p2.normalized() * Thickness + point;
    }

    Game::Get().GetWindow().draw(lines);
}
