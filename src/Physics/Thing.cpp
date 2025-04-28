
#include "Thing.h"

void Thing::SetPosition(const sf::Vector2f& Position)
{
    position = Position;
}

void Thing::Move(const sf::Vector2f& Delta)
{
    position += Delta;
}

void Thing::SetVelocity(const sf::Vector2f& Velocity)
{
    velocity = Velocity;
}

sf::Vector2f Thing::GetPosition() const
{
    return position;
}

sf::Vector2f Thing::GetVelocity() const
{
    return velocity;
}

void Thing::Push(const sf::Vector2f& Force)
{
    velocity += Force;
}

Thing::Thing(float Radius) :
    radius(Radius)
{
}
