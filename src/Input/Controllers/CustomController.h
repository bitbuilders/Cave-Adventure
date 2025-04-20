
#pragma once

#include "Gamepad.h"

class CustomController final : public Gamepad
{
public:
    /// Gamepad interface begin
    void HandleButtonPress(const sf::Event::JoystickButtonPressed& Event, Controls* Controls) override;
    void HandleButtonRelease(const sf::Event::JoystickButtonReleased& Event, Controls* Controls) override;
    void HandleAxis(const sf::Event::JoystickMoved& Event, Controls* Controls) override;
    /// Gamepad interface end

    ~CustomController() override = default;
};
