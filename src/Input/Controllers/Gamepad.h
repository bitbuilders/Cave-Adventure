
#pragma once

#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"

enum class GamepadMode;
class Controls;

/**
 * The base class for specialized controllers (i.e., Xbox, PlayStation, other)
 */
class Gamepad
{
public:
    int player = 0;

public:
    virtual void HandleButtonPress(const sf::Event::JoystickButtonPressed& Event, Controls* Controls) = 0;
    virtual void HandleButtonRelease(const sf::Event::JoystickButtonReleased& Event, Controls* Controls) = 0;
    virtual void HandleAxis(const sf::Event::JoystickMoved& Event, Controls* Controls) = 0;

    virtual void Update(const sf::Time& Delta, Controls* Controls) = 0;

    virtual GamepadMode GetMode() const = 0;

    virtual ~Gamepad() = default;
};
