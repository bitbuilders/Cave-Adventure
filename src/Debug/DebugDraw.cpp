
#include "DebugDraw.h"

#include "Console.h"
#include "Game.h"
#include <cmath>
#include <functional>


namespace DrawDebug
{
    void AddDebugShape(std::function<void(sf::RenderWindow*)> Func, float Duration)
    {
        TimedAction action;
        action.action = [Func](const sf::Time& delta, const sf::Time& lifetime, sf::RenderWindow* window)
        {
            Func(window);
        };
        action.duration = Duration;

        CaveChrono::Get().TrackRenderAction(std::move(action), true);
    }
}


void DrawDebug::Line(const sf::Vector2f& P1, const sf::Vector2f& P2, const sf::Color& Color, float Thickness, float Duration)
{
    sf::VertexArray lines{ sf::PrimitiveType::TriangleStrip, 4 };

    sf::Transform rotation;
    rotation.rotate(sf::degrees(90.0f));
    sf::Vector2f dir = (P2 - P1).normalized();
    sf::Vector2f rotDir = rotation.transformPoint(dir);

    sf::Vector2f padding = dir * Thickness * 0.25f;

    sf::Vector2f p1a = P1 + rotDir * Thickness * 0.5f - padding;
    sf::Vector2f p1b = P1 + -rotDir * Thickness * 0.5f - padding;
    sf::Vector2f p2a = P2 + rotDir * Thickness * 0.5f + padding;
    sf::Vector2f p2b = P2 + -rotDir * Thickness * 0.5f + padding;

    lines[0].color = Color;
    lines[0].position = p1a;
    lines[1].color = Color;
    lines[1].position = p1b;
    lines[2].color = Color;
    lines[2].position = p2a;
    lines[3].color = Color;
    lines[3].position = p2b;

    auto drawLine = [lines](sf::RenderWindow* window)
    {
        window->draw(lines);
    };

    AddDebugShape(drawLine, Duration);
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

void DrawDebug::Rect(const sf::Vector2f& Center, const sf::Vector2f& Extent, const sf::Color& Color, float Thickness, float Duration)
{
    sf::RectangleShape shape(Extent * 2.0f);
    shape.setPosition(Center - Extent);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(Color);
    shape.setOutlineThickness(-Thickness);

    auto drawRect = [shape](sf::RenderWindow* window)
    {
        window->draw(shape);
    };

    AddDebugShape(drawRect, Duration);
}

void DrawDebug::Circle(const sf::Vector2f& Point, float Radius, const sf::Color& Color, float Thickness, int Segments, float Duration)
{
    sf::CircleShape shape(Radius, Segments);
    shape.setPosition(Point);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(Color);
    shape.setOutlineThickness(-Thickness);

    auto drawCircle = [shape](sf::RenderWindow* window)
    {
        window->draw(shape);
    };

    AddDebugShape(drawCircle, Duration);
}
