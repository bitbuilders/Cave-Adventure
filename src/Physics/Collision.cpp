
#include "Collision.h"

#include "CaveChrono.h"
#include "CaveRand.h"
#include "GameConfig.h"
#include "ModuleContainer.h"

MAKE_STARTUP_MODULE(Collision)

void Collision::Init()
{
    TimedAction renderThings;
    renderThings.infinite = true;
    renderThings.action = [this](const sf::Time&, const sf::Time&, sf::RenderWindow* Window)
    {
        RenderThings(Window);
    };

    CaveChrono::Get().TrackRenderAction(renderThings);

    for (int i = 0; i < 10; ++i)
    {
        auto thing = things.emplace_back(std::make_shared<Thing>());
        thing->SetPosition({i * 100.0f, 50.0f});
    }
}

void Collision::FixedUpdate(float FixedDelta)
{
    auto gravity = GameConfig::Gravity * FixedDelta;

    for (auto thing : things)
    {
        thing->Push(gravity * CaveRand::Get().Range(0.9f, 1.1f));

        thing->position += thing->velocity * FixedDelta;

        if (thing->position.y >= 900.0f && thing->velocity.y > 0.0f)
        {
            thing->velocity.y *= -1.0f * 0.9f;
        }
    }
}

void Collision::RenderThings(sf::RenderWindow* Window)
{
    for (auto thing : things)
    {
        sf::CircleShape circle(thing->radius);
        circle.setFillColor(sf::Color::White);
        circle.setPosition(thing->GetPosition() - sf::Vector2f(thing->radius, thing->radius));

        Window->draw(circle);
    }
}
