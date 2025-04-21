
#pragma once

#include "SFML/Window/Event.hpp"
#include "SFML/System/Vector2.hpp"


class Controls;

class Mouse
{
    sf::Vector2i lastPos;

public:
    void HandleButtonPress(const sf::Event::MouseButtonPressed& Event, Controls* Controls);
    void HandleButtonRelease(const sf::Event::MouseButtonReleased& Event, Controls* Controls);
    void HandleMove(const sf::Event::MouseMoved& Event, Controls* Controls);
    void HandleScroll(const sf::Event::MouseWheelScrolled& Event, Controls* Controls);

    void ResetAxes(Controls* Controls);

    friend Controls;
};
