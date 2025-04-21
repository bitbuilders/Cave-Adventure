
#pragma once

#include "Gamepad.h"

class XboxController final : public Gamepad
{
public:
    /// Gamepad interface start
    void HandleButtonPress(const sf::Event::JoystickButtonPressed& Event, Controls* Controls) override;

    void HandleButtonRelease(const sf::Event::JoystickButtonReleased& Event, Controls* Controls) override;

    void HandleAxis(const sf::Event::JoystickMoved& Event, Controls* Controls) override;

    void Update(const sf::Time& Delta, Controls* Controls) override;

    GamepadMode GetMode() const override;
    /// Gamepad interface end

    XboxController();
    ~XboxController() override = default;
};
