
#include "CustomController.h"

#include "Controls.h"

void CustomController::HandleButtonPress(const sf::Event::JoystickButtonPressed& Event, Controls* Controls)
{
}

void CustomController::HandleButtonRelease(const sf::Event::JoystickButtonReleased& Event, Controls* Controls)
{
}

void CustomController::HandleAxis(const sf::Event::JoystickMoved& Event, Controls* Controls)
{
}

void CustomController::Update(const sf::Time& Delta, Controls* Controls)
{
}

GamepadMode CustomController::GetMode() const
{
    return GamepadMode::Custom;
}
