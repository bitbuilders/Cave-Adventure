
#include "Mouse.h"

#include <cmath>

#include "Controls.h"

void Mouse::HandleButtonPress(const sf::Event::MouseButtonPressed& Event, Controls* Controls)
{
    PressState state;
    state.down = true;

    Controls->SetPressedState(Event.button, state);
}

void Mouse::HandleButtonRelease(const sf::Event::MouseButtonReleased& Event, Controls* Controls)
{
    PressState state;
    state.down = false;

    Controls->SetPressedState(Event.button, state);
}

void Mouse::HandleMove(const sf::Event::MouseMoved& Event, Controls* Controls)
{
    sf::Vector2i delta = Event.position - lastPos;

    if (std::abs(delta.x) > 0)
    {
        AxisState moveX;
        moveX.value = static_cast<float>(Event.position.x);
        Controls->SetAxisState(MouseAxis::X, moveX);

        lastPos.x = Event.position.x;
    }

    if (std::abs(delta.y) > 0)
    {
        AxisState moveY;
        moveY.value = static_cast<float>(Event.position.y);
        Controls->SetAxisState(MouseAxis::Y, moveY);

        lastPos.y = Event.position.y;
    }
}

void Mouse::HandleScroll(const sf::Event::MouseWheelScrolled& Event, Controls* Controls)
{
    AxisState state;
    state.value = Event.delta;

    Controls->SetAxisState(MouseAxis::Wheel, state);
}

void Mouse::ResetAxes(Controls* Controls)
{
    // AxisState moveX;
    // moveX.value = 0.0f;
    // Controls->SetAxisState(MouseAxis::X, moveX, KBM_PLAYER, false);
    //
    // AxisState moveY;
    // moveX.value = 0.0f;
    // Controls->SetAxisState(MouseAxis::Y, moveY, KBM_PLAYER, false);

    AxisState scroll;
    scroll.value = 0.0f;
    Controls->SetAxisState(MouseAxis::Wheel, scroll, KBM_PLAYER, false);
}
