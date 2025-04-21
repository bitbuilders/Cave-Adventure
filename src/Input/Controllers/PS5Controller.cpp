
#include "PS5Controller.h"

#include <map>

#include "Calc.h"
#include "Controls.h"


/*
 *--------------------------------------
 *  JS button   |   PS5 button
 *--------------------------------------
 *  0           | ◻
 *  1           | X
 *  2           | O
 *  3           | △
 *  4           | LB
 *  5           | RB
 *  6           |
 *  7           |
 *  8           | Special
 *  9           | Menu
 *  10          | LS
 *  11          | RS
 *  12          | Logo
 *  13          | Touchpad
 *  14          | Mute
 */

/*
 *--------------------------------------
 *  JS axis     |   PS5 axis
 *--------------------------------------
 *  0 (X)       | LSX
 *  1 (Y)       | LSY
 *  2 (Z)       | RSX
 *  3 (R)       | RSY
 *  4 (U)       | LT
 *  5 (V)       | RT
 *  6 (PovX)    | D-pad X
 *  7 (PovY)    | D-pad Y
 */

namespace PS5
{
    std::map<int, GamepadButton> mappedButtons =
        {
        {0, GamepadButton::West},
        {1, GamepadButton::South},
        {2, GamepadButton::East},
        {3, GamepadButton::North},
        {4, GamepadButton::LB},
        {5, GamepadButton::RB},
        {8, GamepadButton::Special},
        {9, GamepadButton::Menu},
        {10, GamepadButton::LS},
        {11, GamepadButton::RS},
        {12, GamepadButton::Logo}
        };

    std::map<int, GamepadAxis> mappedAxes =
        {
        {0, GamepadAxis::LSX},
        {1, GamepadAxis::LSY},
        {2, GamepadAxis::RSX},
        {3, GamepadAxis::RSY},
        {4, GamepadAxis::LT},
        {5, GamepadAxis::RT}
        };

    constexpr auto dpadX = sf::Joystick::Axis::PovX;
    constexpr auto dpadY = sf::Joystick::Axis::PovY;
}

void PS5Controller::HandleButtonPress(const sf::Event::JoystickButtonPressed& Event, Controls* Controls)
{
    auto joystick = static_cast<int>(Event.joystickId);
    if (joystick != player)
    {
        return;
    }

    auto button = static_cast<int>(Event.button);
    if (!PS5::mappedButtons.contains(button))
    {
        return;
    }

    PressState state;
    state.down = true;
    auto buttonMap = PS5::mappedButtons[button];
    Controls->SetPressedState(buttonMap, state, player);
}

void PS5Controller::HandleButtonRelease(const sf::Event::JoystickButtonReleased& Event, Controls* Controls)
{
    auto joystick = static_cast<int>(Event.joystickId);
    if (joystick != player)
    {
        return;
    }

    auto button = static_cast<int>(Event.button);
    if (!PS5::mappedButtons.contains(button))
    {
        return;
    }

    PressState state;
    state.down = false;
    auto buttonMap = PS5::mappedButtons[button];
    Controls->SetPressedState(buttonMap, state, player);
}

void PS5Controller::HandleAxis(const sf::Event::JoystickMoved& Event, Controls* Controls)
{
    auto joystick = static_cast<int>(Event.joystickId);
    if (joystick != player)
    {
        return;
    }

    AxisState state {Event.position};

    auto axisIndex = static_cast<int>(Event.axis);
    if (PS5::mappedAxes.contains(axisIndex))
    {
        auto axisMap = PS5::mappedAxes[axisIndex];
        Controls->SetAxisState(axisMap, state, player);
    }
    else
    {
        switch (Event.axis)
        {
        case PS5::dpadX:
            {
                PressState buttonState;
                buttonState.down = !Math::NearlyZero(state.value);

                auto dpadButton = state.value > 0.0f ? GamepadButton::Right : GamepadButton::Left;
                auto dpadButtonInv = state.value > 0.0f ? GamepadButton::Left : GamepadButton::Right;

                if (buttonState.down)
                {
                    Controls->SetPressedState(dpadButton, buttonState, player);
                    PressState buttonStateInv;
                    buttonStateInv.down = false;
                    Controls->SetPressedState(dpadButtonInv, buttonStateInv, player);
                }
                else
                {
                    Controls->SetPressedState(GamepadButton::Left, buttonState, player);
                    Controls->SetPressedState(GamepadButton::Right, buttonState, player);
                }
            }
            break;
        case PS5::dpadY:
            {
                PressState buttonState;
                buttonState.down = !Math::NearlyZero(state.value);

                auto dpadButton = state.value > 0.0f ? GamepadButton::Up : GamepadButton::Down;
                auto dpadButtonInv = state.value > 0.0f ? GamepadButton::Down : GamepadButton::Up;

                if (buttonState.down)
                {
                    Controls->SetPressedState(dpadButton, buttonState, player);
                    PressState buttonStateInv;
                    buttonStateInv.down = false;
                    Controls->SetPressedState(dpadButtonInv, buttonStateInv, player);
                }
                else
                {
                    Controls->SetPressedState(GamepadButton::Up, buttonState, player);
                    Controls->SetPressedState(GamepadButton::Down, buttonState, player);
                }
            }
            break;
        default:
            break;
        }
    }
}

void PS5Controller::Update(const sf::Time& Delta, Controls* Controls)
{
}

GamepadMode PS5Controller::GetMode() const
{
    return GamepadMode::PS5;
}
