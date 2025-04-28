
#pragma once

#include <memory>

#include "SFML/System/Vector2.hpp"

class Module;

class GameConfig
{
public:
    /** How often physics happens */
    static constexpr float FixedUpdateInterval = 1.0f / 80.0f;

    /** Number of pixels to one unit */
    static constexpr float Bep = 100.0f;

    /** The default gravitational acceleration, in pixels */
    static constexpr sf::Vector2f Gravity = sf::Vector2f(0.0f, Bep * 2.0f);
};
