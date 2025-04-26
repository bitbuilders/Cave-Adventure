
#include "CaveGameInput.h"

#include "Controls.h"

#include <GameInput.h>

#include <vector>
#include <map>

#include "Calc.h"


namespace Input
{
    GameInput::v1::IGameInput* g_gameInput = nullptr;
    std::vector<GameInput::v1::IGameInputDevice*> g_gamepads(MAX_PLAYERS, nullptr);

    constexpr auto sonyVendorId = 0x054C;
    constexpr auto xboxVendorId = 0x045E;

    GameInput::v1::GameInputMouseState lastMouseState;

    std::map<GameInput::v1::GameInputGamepadButtons, GamepadButton> xboxButtonMap =
        {
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadA, GamepadButton::South},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadB, GamepadButton::East},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadX, GamepadButton::West},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadY, GamepadButton::North},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadDPadUp, GamepadButton::Up},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadDPadDown, GamepadButton::Down},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadDPadLeft, GamepadButton::Left},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadDPadRight, GamepadButton::Right},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadLeftShoulder, GamepadButton::LB},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadRightShoulder, GamepadButton::RB},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadLeftThumbstick, GamepadButton::LS},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadRightThumbstick, GamepadButton::RS},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadMenu, GamepadButton::Menu},
        {GameInput::v1::GameInputGamepadButtons::GameInputGamepadView, GamepadButton::Special}
        };

    std::map<GameInput::v1::GameInputMouseButtons, sf::Mouse::Button> mouseButtonMap =
        {
        {GameInput::v1::GameInputMouseButtons::GameInputMouseLeftButton, sf::Mouse::Button::Left},
        {GameInput::v1::GameInputMouseButtons::GameInputMouseMiddleButton, sf::Mouse::Button::Middle},
        {GameInput::v1::GameInputMouseButtons::GameInputMouseRightButton, sf::Mouse::Button::Right},
        {GameInput::v1::GameInputMouseButtons::GameInputMouseButton4, sf::Mouse::Button::Extra1},
        {GameInput::v1::GameInputMouseButtons::GameInputMouseButton5, sf::Mouse::Button::Extra2}
        };

    constexpr uint32_t maxKeyCount = 16;

    void ProcessKeyboard(Controls* Controls)
    {
        // This works but not really worth it, just using SFML because values are easily referenced by enum (see Controls::Poll)

        // GameInput::v1::IGameInputReading* reading;
        // auto result = g_gameInput->GetCurrentReading(GameInput::v1::GameInputKindKeyboard, nullptr, &reading);
        // if (SUCCEEDED(result))
        // {
        //     GameInput::v1::GameInputKeyState keyState[maxKeyCount];
        //     reading->GetKeyState(maxKeyCount, keyState);
        //     reading->Release();
        //
        //     for (uint32_t key = 0; key < reading->GetKeyCount(); ++key)
        //     {
        //         LOG("Pressed key {}", static_cast<char>(keyState[key].virtualKey));
        //     }
        // }
    }

    void ProcessMouse(Controls* Controls)
    {
        GameInput::v1::IGameInputReading* reading;
        auto result = g_gameInput->GetCurrentReading(GameInput::v1::GameInputKindMouse, nullptr, &reading);
        if (SUCCEEDED(result))
        {
            GameInput::v1::GameInputMouseState state{};
            reading->GetMouseState(&state);
            reading->Release();

            auto deltaX = static_cast<float>(state.positionX - lastMouseState.positionX);
            auto deltaY = static_cast<float>(state.positionY - lastMouseState.positionY);
            auto deltaWheel = static_cast<float>(state.wheelY - lastMouseState.wheelY);
            deltaWheel = Math::Sign(deltaWheel);

            lastMouseState = state;

            Controls->SetAxisState(MouseAxis::X, {deltaX}, KBM_PLAYER);
            Controls->SetAxisState(MouseAxis::Y, {deltaY}, KBM_PLAYER);
            Controls->SetAxisState(MouseAxis::Wheel, {deltaWheel}, KBM_PLAYER);

            for (const auto & buttonMap : mouseButtonMap)
            {
                PressState pressState;
                if (state.buttons & buttonMap.first)
                {
                    pressState.down = true;
                }
                Controls->SetPressedState(buttonMap.second, pressState, KBM_PLAYER);
            }
        }
    }

    void ProcessGamepads(Controls* Controls)
    {
        constexpr int player = 0;
        GameInput::v1::IGameInputReading* reading;
        auto result = g_gameInput->GetCurrentReading(GameInput::v1::GameInputKindGamepad, g_gamepads[player], &reading);
        if (SUCCEEDED(result))
        {
            if (!g_gamepads[player])
            {
                reading->GetDevice(&g_gamepads[player]);
            }

            bool isXbox = false;
            bool isPlayStation = false;
            const GameInput::v1::GameInputDeviceInfo* info;
            if (SUCCEEDED(g_gamepads[player]->GetDeviceInfo(&info)))
            {
                isXbox = info->vendorId == xboxVendorId;
                isPlayStation = info->vendorId == sonyVendorId;
            }

            if (isXbox)
            {
                Controls->SetGamepadType(player, GamepadType::Xbox);
            }
            else if (isPlayStation)
            {
                Controls->SetGamepadType(player, GamepadType::PS5);
            }
            else
            {
                Controls->SetGamepadType(player, GamepadType::Other);
            }

            GameInput::v1::GameInputGamepadState state{};
            reading->GetGamepadState(&state);
            reading->Release();

            for (const auto& buttons : xboxButtonMap)
            {
                PressState pressState;
                if (state.buttons & buttons.first)
                {
                    pressState.down = true;
                }
                Controls->SetPressedState(buttons.second, pressState, player);
            }

            // very hacky, but I want controls to be consistent across controllers
            float yAxisDir = isPlayStation ? -1.0f : 1.0f;

            Controls->SetAxisState(GamepadAxis::LSX, {state.leftThumbstickX}, player);
            Controls->SetAxisState(GamepadAxis::LSY, {state.leftThumbstickY * yAxisDir}, player);
            Controls->SetAxisState(GamepadAxis::RSX, {state.rightThumbstickX}, player);
            Controls->SetAxisState(GamepadAxis::RSY, {state.rightThumbstickY * yAxisDir}, player);
            Controls->SetAxisState(GamepadAxis::LT, {state.leftTrigger}, player);
            Controls->SetAxisState(GamepadAxis::RT, {state.rightTrigger}, player);
        }
        else if (g_gamepads[player])
        {
            g_gamepads[player]->Release();
            g_gamepads[player] = nullptr;
        }
    }
}


bool Input::Init()
{
    auto result = GameInput::v1::GameInputCreate(&g_gameInput);

    return SUCCEEDED(result) && g_gameInput;
}

void Input::Shutdown()
{
    for (auto g_gamepad : g_gamepads)
    {
        if (g_gamepad)
        {
            g_gamepad->Release();
        }
    }

    if (g_gameInput)
    {
        g_gameInput->Release();
    }
}

void Input::ProcessInput(Controls* Controls)
{
    if (!g_gameInput)
    {
        return;
    }

    ProcessKeyboard(Controls);

    ProcessMouse(Controls);

    ProcessGamepads(Controls);
}
