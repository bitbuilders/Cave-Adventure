
#pragma once

#include <SFML/System/Vector2.hpp>

class Thing
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    float radius;

public:
    void SetPosition(const sf::Vector2f& Position);

    void Move(const sf::Vector2f& Delta);

    void SetVelocity(const sf::Vector2f& Velocity);

    sf::Vector2f GetPosition() const;

    sf::Vector2f GetVelocity() const;

    void Push(const sf::Vector2f& Force);

    Thing(float Radius = 30.0f);

    friend class Collision;
};
