
#include "Keyboard.h"

#include "Controls.h"

void Keyboard::HandleKeyPress(const sf::Event::KeyPressed& Event, Controls* Controls) const
{
    PressState state;
    state.down = true;

    Controls->SetPressedState(Event.code, state);
}

void Keyboard::HandleKeyRelease(const sf::Event::KeyReleased& Event, Controls* Controls) const
{
    PressState state;
    state.down = false;

    Controls->SetPressedState(Event.code, state);
}
