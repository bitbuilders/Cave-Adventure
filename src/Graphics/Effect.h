
#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

struct TimedPoint
{
    sf::Vector2f position;
    sf::Vector2f dir;

    sf::Color color;

    float size = 10.0f;
    float speed = 1.0f;
    float time = 0.0f;
    float duration = 0.0f;
};

class Effect : public sf::Drawable, public sf::Transformable
{
    sf::VertexArray particles;
    std::vector<TimedPoint> points;

    float time;

public:
    void Update(const sf::Time& Delta);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    Effect();
};
