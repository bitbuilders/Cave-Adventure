
#pragma once

#include "SFML/Window/Event.hpp"


class Controls;

class Keyboard final
{
public:
    void HandleKeyPress(const sf::Event::KeyPressed& Event, Controls* Controls) const;
    void HandleKeyRelease(const sf::Event::KeyReleased& Event, Controls* Controls) const;
};
