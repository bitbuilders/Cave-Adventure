
#include "DebugDraw.h"

#include "Console.h"
#include "Game.h"
#include <cmath>

void DrawDebug::Line(const sf::Vector2f& P1, const sf::Vector2f& P2, const sf::Color& Color, float Thickness, float Duration)
{
    auto renderLine = [=](sf::RenderWindow* window)
    {
        sf::VertexArray lines{ sf::PrimitiveType::TriangleStrip, 4 };

        sf::Transform rotation;
        rotation.rotate(sf::degrees(90.0f));
        sf::Vector2f dir = (P2 - P1).normalized();
        sf::Vector2f rotDir = rotation.transformPoint(dir);

        sf::Vector2f padding = dir * Thickness * 0.5f;
        if (padding.lengthSquared() >= std::pow(2.0f, 2.0f))
        {
            // this makes them look better
            padding -= dir * 1.0f;
        }

        sf::Vector2f p1a = P1 + rotDir * Thickness - padding;
        sf::Vector2f p1b = P1 + -rotDir * Thickness - padding;
        sf::Vector2f p2a = P2 + rotDir * Thickness + padding;
        sf::Vector2f p2b = P2 + -rotDir * Thickness + padding;

        lines[0].color = Color;
        lines[0].position = p1a;
        lines[1].color = Color;
        lines[1].position = p1b;
        lines[2].color = Color;
        lines[2].position = p2a;
        lines[3].color = Color;
        lines[3].position = p2b;

        window->draw(lines);
    };

    if (Duration > 0)
    {
        TimedAction action;
        action.action = [renderLine](const sf::Time& delta, const sf::Time& lifetime, sf::RenderWindow* window)
        {
            renderLine(window);
        };
        action.duration = Duration;

        CaveChrono::Get().TrackRenderAction(std::move(action), true);
    }
    else
    {
        renderLine(&Game::Get().GetWindow());
    }
}

void DrawDebug::LineSegment(const std::vector<sf::Vector2f>& Points, const sf::Color& Color, float Thickness, float Duration)
{
    if (Points.size() < 2)
    {
        LOG_WARN("Tried to make a line with less than 2 points!");
        return;
    }

    for (int i = 0; i < Points.size() - 1; i++)
    {
        Line(Points[i], Points[i + 1], Color, Thickness, Duration);
    }
}
