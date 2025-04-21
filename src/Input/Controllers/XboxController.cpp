
#include "XboxController.h"

#include <map>

#include "Calc.h"
#include "Controls.h"

#include <GameInput.h>

/*
 *--------------------------------------
 *  JS button   |   Xbox button
 *--------------------------------------
 *  0           | A
 *  1           | B
 *  2           | X
 *  3           | Y
 *  4           | LB
 *  5           | RB
 *  6           | Special
 *  7           | Menu
 *  8           | LS
 *  9           | RS
 *  10          | Logo
 */

/*
 *--------------------------------------
 *  JS axis     |   Xbox axis
 *--------------------------------------
 *  0 (X)       | LSX
 *  1 (Y)       | LSY
 *  2 (Z)       | <0,100> = LT, <0,-100> RT
 *  3 (R)       | ???
 *  4 (U)       | RSX
 *  5 (V)       | RSY
 *  6 (PovX)    | D-pad X
 *  7 (PovY)    | D-pad Y
 */

// todo: do something else for xbox controller input (GameInput looks promising)
// Crazy how SFML doesn't support individual triggers

namespace Xbox
{
    GameInput::v1::IGameInput* g_gameInput = nullptr;
    GameInput::v1::IGameInputDevice* g_gamepad = nullptr;

    std::map<int, GamepadButton> buttonMapping =
        {
        { 0, GamepadButton::South },
        { 1, GamepadButton::East },
        { 2, GamepadButton::West },
        { 3, GamepadButton::North },
        { 4, GamepadButton::LB },
        { 5, GamepadButton::RB },
        { 6, GamepadButton::Special },
        { 7, GamepadButton::Menu },
        { 8, GamepadButton::LS },
        { 9, GamepadButton::RS },
        { 10, GamepadButton::Logo }
        };

    std::map<int, GamepadAxis> axisMapping =
        {
        { 0, GamepadAxis::LSX },
        { 1, GamepadAxis::LSY },
        { 4, GamepadAxis::RSX },
        { 5, GamepadAxis::RSY }
        };

    constexpr auto triggerAxis = sf::Joystick::Axis::Z; // <0,100> = LT, <0,-100> RT
    constexpr auto dpadXAxis = sf::Joystick::Axis::PovX;
    constexpr auto dpadYAxis = sf::Joystick::Axis::PovY;
}

void XboxController::HandleButtonPress(const sf::Event::JoystickButtonPressed& Event, Controls* Controls)
{
    // auto joystick = static_cast<int>(Event.joystickId);
    // if (joystick != player)
    // {
    //     return;
    // }
    //
    // auto buttonIndex = static_cast<int>(Event.button);
    // if (!Xbox::buttonMapping.contains(buttonIndex))
    // {
    //     return;
    // }
    //
    // PressState state;
    // state.down = true;
    //
    // auto button = Xbox::buttonMapping[buttonIndex];
    // Controls->SetPressedState(button, state, joystick);
}

void XboxController::HandleButtonRelease(const sf::Event::JoystickButtonReleased& Event, Controls* Controls)
{
    // auto joystick = static_cast<int>(Event.joystickId);
    // if (joystick != player)
    // {
    //     return;
    // }
    //
    // auto buttonIndex = static_cast<int>(Event.button);
    // if (!Xbox::buttonMapping.contains(buttonIndex))
    // {
    //     return;
    // }
    //
    // PressState state;
    // state.down = false;
    //
    // auto button = Xbox::buttonMapping[buttonIndex];
    // Controls->SetPressedState(button, state, joystick);
}

void XboxController::HandleAxis(const sf::Event::JoystickMoved& Event, Controls* Controls)
{
    // auto joystick = static_cast<int>(Event.joystickId);
    // if (joystick != player)
    // {
    //     return;
    // }
    //
    // AxisState state {Event.position};
    //
    // auto axisIndex = static_cast<int>(Event.axis);
    // if (Xbox::axisMapping.contains(axisIndex))
    // {
    //     auto axis = Xbox::axisMapping[axisIndex];
    //     Controls->SetAxisState(axis, state, joystick);
    // }
    // else
    // {
    //     switch (Event.axis)
    //     {
    //     case Xbox::triggerAxis:
    //         {
    //             // GamepadAxis trigger = state.value > 0.0f ? GamepadAxis::LT : GamepadAxis::RT;
    //             // state.value = std::fabs(state.value);
    //             // Controls->SetAxisState(trigger, state, joystick);
    //         }
    //         break;
    //     case Xbox::dpadXAxis:
    //         {
    //             PressState buttonState;
    //             buttonState.down = !Math::NearlyZero(state.value);
    //
    //             if (buttonState.down)
    //             {
    //                 GamepadButton downButton = state.value > 0.0f ? GamepadButton::Right : GamepadButton::Left;
    //                 Controls->SetPressedState(downButton, buttonState, joystick);
    //
    //                 PressState upState;
    //                 upState.down = false;
    //                 GamepadButton upButton = state.value > 0.0f ? GamepadButton::Left : GamepadButton::Right;
    //                 Controls->SetPressedState(upButton, upState, joystick);
    //             }
    //             else
    //             {
    //                 Controls->SetPressedState(GamepadButton::Right, buttonState, joystick);
    //                 Controls->SetPressedState(GamepadButton::Left, buttonState, joystick);
    //             }
    //         }
    //         break;
    //     case Xbox::dpadYAxis:
    //         {
    //             PressState buttonState;
    //             buttonState.down = !Math::NearlyZero(state.value);
    //
    //             if (buttonState.down)
    //             {
    //                 GamepadButton downButton = state.value > 0.0f ? GamepadButton::Up : GamepadButton::Down;
    //                 Controls->SetPressedState(downButton, buttonState, joystick);
    //
    //                 PressState upState;
    //                 upState.down = false;
    //                 GamepadButton upButton = state.value > 0.0f ? GamepadButton::Down : GamepadButton::Up;
    //                 Controls->SetPressedState(upButton, upState, joystick);
    //             }
    //             else
    //             {
    //                 Controls->SetPressedState(GamepadButton::Up, buttonState, joystick);
    //                 Controls->SetPressedState(GamepadButton::Down, buttonState, joystick);
    //             }
    //         }
    //         break;
    //     default:
    //         break;
    //     }
    // }
}

void XboxController::Update(const sf::Time& Delta, Controls* Controls)
{
    // auto triggerVal = sf::Joystick::getAxisPosition(player, Xbox::triggerAxis);
    //
    // GamepadAxis trigger = triggerVal > 0.0f ? GamepadAxis::LT : GamepadAxis::RT;
    // triggerVal = std::fabs(triggerVal);
    //
    // AxisState state = { triggerVal };
    // Controls->SetAxisState(trigger, state, player);

    if (!Xbox::g_gameInput)
    {
        return;
    }

    GameInput::v1::IGameInputReading * reading;
    if (SUCCEEDED(Xbox::g_gameInput->GetCurrentReading(GameInput::v1::GameInputKindGamepad, Xbox::g_gamepad, &reading)))
    {
        // If no device has been assigned to g_gamepad yet, set it
        // to the first device we receive input from. (This must be
        // the one the player is using because it's generating input.)
        if (!Xbox::g_gamepad) reading->GetDevice(&Xbox::g_gamepad);

        // Retrieve the fixed-format gamepad state from the reading.
        GameInput::v1::GameInputGamepadState state;
        reading->GetGamepadState(&state);
        reading->Release();
        const GameInput::v1::GameInputDeviceInfo* info;
        Xbox::g_gamepad->GetDeviceInfo(&info);

        // Application-specific code to process the gamepad state goes here.
        if (state.buttons & GameInput::v1::GameInputGamepadA)
        {
            LOG("GAMEINPUT A press!");
        }
    }

    // If an error is returned from GetCurrentReading(), it means the
    // gamepad we were reading from has disconnected. Reset the
    // device pointer, and go back to looking for an active gamepad.
    else if (Xbox::g_gamepad)
    {
        Xbox::g_gamepad->Release();
        Xbox::g_gamepad = nullptr;
    }
}

GamepadMode XboxController::GetMode() const
{
    return GamepadMode::Xbox;
}

XboxController::XboxController()
{
    auto result = GameInput::v1::GameInputCreate(&Xbox::g_gameInput);
    LOG("{} {}", Xbox::g_gameInput ? "valid" : "invalid", result);
}
