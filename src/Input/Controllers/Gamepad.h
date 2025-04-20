
#pragma once

#include "SFML/Window/Event.hpp"

class Controls;

/**
 * The base class for specialized controllers (i.e., Xbox, PlayStation, other)
 */
class Gamepad
{
public:
    virtual void HandleButtonPress(const sf::Event::JoystickButtonPressed& Event, Controls* Controls) = 0;
    virtual void HandleButtonRelease(const sf::Event::JoystickButtonReleased& Event, Controls* Controls) = 0;
    virtual void HandleAxis(const sf::Event::JoystickMoved& Event, Controls* Controls) = 0;

    virtual ~Gamepad() = default;
};
