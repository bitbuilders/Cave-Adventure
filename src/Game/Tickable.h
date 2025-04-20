
#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Time.hpp"

class Tickable
{
public:
    virtual ~Tickable() = default;

    virtual void Update(const sf::Time& Delta) = 0;

    virtual void LateUpdate(const sf::Time& Delta) = 0;

    virtual void Render(sf::RenderWindow& Window) = 0;

    virtual void Poll(const sf::Event& Event) = 0;

    virtual constexpr bool CanUpdate() = 0;

    virtual constexpr bool CanLateUpdate() = 0;

    virtual constexpr bool CanRender() = 0;

    virtual constexpr bool CanPoll() = 0;
};
