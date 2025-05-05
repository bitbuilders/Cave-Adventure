
#include "Controls.h"

#include "Game.h"

#include <algorithm>
#include <ranges>

#include "Calc.h"
#include "CaveGameInput.h"
#include "InputViewer.h"
#include "ModuleContainer.h"

MAKE_STARTUP_MODULE(Controls)

CallbackHandle Controls::currentHandle = 0;

Controls& Controls::Get()
{
    return ModuleContainer::Get().LoadModule<Controls>("Controls");
}

CallbackHandle Controls::ListenForPress(PressCallback& Callback)
{
    auto callback = pressListeners.emplace_back(Callback);
    callback.handle = currentHandle++;

    return callback.handle;
}

CallbackHandle Controls::ListenForAxis(AxisCallback& Callback)
{
    auto callback = axisListeners.emplace_back(Callback);
    callback.handle = currentHandle++;

    return callback.handle;
}

void Controls::ChangePressedMappings(const std::vector<PressedMapping>& PressedMappings, CallbackHandle Handle)
{
    for (auto& listener : pressListeners)
    {
        if (listener.handle == Handle)
        {
            listener.mappings = PressedMappings;
        }
    }
}

void Controls::ChangeAxisMappings(const std::vector<AxisMapping>& AxisMappings, CallbackHandle Handle)
{
    for (auto& listener : axisListeners)
    {
        if (listener.handle == Handle)
        {
            listener.mappings = AxisMappings;
        }
    }
}

void Controls::RemoveListener(CallbackHandle Handle)
{
    auto removedPress = std::ranges::remove(pressListeners, Handle, &PressCallback::handle);
    if (!removedPress.empty())
    {
        pressListeners.erase(removedPress.begin(), removedPress.end());
    }

    auto removedAxis = std::ranges::remove(axisListeners, Handle, &AxisCallback::handle);
    if (!removedPress.empty())
    {
        axisListeners.erase(removedAxis.begin(), removedAxis.end());
    }
}

bool Controls::GetPressedState(const PressedVariant& pressed, PressedInputType::Type type, int player ) const
{
    if (pressed.valueless_by_exception())
    {
        return false;
    }

    auto checkState = [type](const PressState& PressState)
    {
        return (type & PressedInputType::Pressed && PressState.pressed) ||
            (type & PressedInputType::Released && PressState.released) ||
            (type & PressedInputType::Down && PressState.down) ||
            (type & PressedInputType::Up && !PressState.down);
    };

    if (auto key = std::get_if<sf::Keyboard::Key>(&pressed))
    {
        if (player == KBM_PLAYER)
        {
            for (const auto& keyboardKey : keyboardKeys)
            {
                if (keyboardKey.key == *key)
                {
                    if (checkState(keyboardKey.state))
                    {
                        return true;
                    }
                }
            }
        }
    }
    else if (auto mouseButton = std::get_if<sf::Mouse::Button>(&pressed))
    {
        if (player == KBM_PLAYER)
        {
            for (const auto& button : mouseButtons)
            {
                if (button.button == *mouseButton)
                {
                    if (checkState(button.state))
                    {
                        return true;
                    }
                }
            }
        }
    }
    else if (auto gamepadButton = std::get_if<GamepadButton>(&pressed))
    {
        for (const auto& button : gamepadButtons)
        {
            if (button.button == *gamepadButton && button.player == player)
            {
                if (checkState(button.state))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

float Controls::GetAxis(const AxisVariant& axis, int player) const
{
    if (axis.valueless_by_exception())
    {
        return 0.0f;
    }

    if (auto mouse = std::get_if<MouseAxis>(&axis))
    {
        if (player == KBM_PLAYER)
        {
            for (auto mouseAxis : mouseAxes)
            {
                if (mouseAxis.axis == *mouse)
                {
                    return mouseAxis.state.value;
                }
            }
        }
    }
    else if (auto gamepad = std::get_if<GamepadAxis>(&axis))
    {
        for (auto gamepadAxis : gamepadAxes)
        {
            if (gamepadAxis.axis == *gamepad && gamepadAxis.player == player)
            {
                return gamepadAxis.state.value;
            }
        }
    }

    return 0.0f;
}

void Controls::SetPressedState(const PressedVariant& Pressed, const PressState& State, int player, bool Broadcast)
{
    if (Pressed.valueless_by_exception())
    {
        return;
    }

    auto setState = [this, State, Pressed, player, Broadcast](PressState& CurrentState)
    {
        if (!CurrentState.down && State.down)
        {
            CurrentState.pressed = true;
            if (Broadcast)
            {
                BroadcastPressed(Pressed, PressedInputType::Pressed, player);
            }
        }
        else if (CurrentState.down && !State.down)
        {
            CurrentState.released = true;
            if (Broadcast)
            {
                BroadcastPressed(Pressed, PressedInputType::Released, player);
            }
        }

        CurrentState.down = State.down;
    };

    if (auto keyboardKey = std::get_if<sf::Keyboard::Key>(&Pressed))
    {
        if (player == KBM_PLAYER)
        {
            for (auto& key : keyboardKeys)
            {
                if (key.key == *keyboardKey)
                {
                    setState(key.state);
                }
            }
        }
    }
    else if (auto mouseButton = std::get_if<sf::Mouse::Button>(&Pressed))
    {
        if (player == KBM_PLAYER)
        {
            for (auto& button : mouseButtons)
            {
                if (button.button == *mouseButton)
                {
                    setState(button.state);
                }
            }
        }
    }
    else if (auto gamepadButton = std::get_if<GamepadButton>(&Pressed))
    {
        for (auto& button : gamepadButtons)
        {
            if (button.button == *gamepadButton && button.player == player)
            {
                setState(button.state);
            }
        }
    }
}

void Controls::SetAxisState(const AxisVariant& Axis, const AxisState& State, int player, bool Broadcast)
{
    if (Axis.valueless_by_exception())
    {
        return;
    }

    auto setState = [this, Axis, player, State, Broadcast](AxisState& CurrentState)
    {
        float old = CurrentState.value;
        CurrentState.value = State.value;
        if (!Math::NearlyEqual(old, CurrentState.value) && Broadcast)
        {
            // LOG("{} <= {}", CurrentState.value, old);
            BroadcastAxis(Axis, AxisInputType::ValueChanged, CurrentState.value, old, player);
        }
    };

    if (auto mouseAxis = std::get_if<MouseAxis>(&Axis))
    {
        if (player == KBM_PLAYER)
        {
            for (auto& axis : mouseAxes)
            {
                if (axis.axis == *mouseAxis)
                {
                    setState(axis.state);
                }
            }
        }
    }
    else if (auto gamepadAxis = std::get_if<GamepadAxis>(&Axis))
    {
        for (auto& axis : gamepadAxes)
        {
            if (axis.axis == *gamepadAxis && axis.player == player)
            {
                setState(axis.state);
            }
        }
    }
}

void Controls::GetDownKeys(std::vector<sf::Keyboard::Key>& Keys) const
{
    for (const auto & key : keyboardKeys)
    {
        if (key.state.down)
        {
            Keys.push_back(key.key);
        }
    }
}

void Controls::SetGamepadType(int Player, GamepadType Type)
{
    if (Math::IsWithinExcl(Player, MAX_PLAYERS))
    {
        gamepads[Player].type = Type;
    }
}

GamepadType Controls::GetGamepadType(int Player) const
{
    if (Math::IsWithinExcl(Player, MAX_PLAYERS))
    {
        return gamepads[Player].type;
    }

    return GamepadType::Other;
}

Controls::~Controls()
{
    Input::Shutdown();
}

void Controls::Init()
{
    Input::Init();

    for (int i = 0; i < sf::Keyboard::KeyCount; ++i)
    {
        keyboardKeys.emplace_back(static_cast<sf::Keyboard::Key>(i));
    }
    for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
    {
        mouseButtons.emplace_back(static_cast<sf::Mouse::Button>(i));
    }
    for (int i = 0; i < static_cast<int>(GamepadButton::COUNT); ++i)
    {
        for (int player = 0; player < MAX_PLAYERS; ++player)
        {
            gamepadButtons.emplace_back(static_cast<GamepadButton>(i), player);
        }
    }
    for (int i = 0; i < static_cast<int>(MouseAxis::COUNT); ++i)
    {
        mouseAxes.emplace_back(static_cast<MouseAxis>(i));
    }
    for (int i = 0; i < static_cast<int>(GamepadAxis::COUNT); ++i)
    {
        for (int player = 0; player < MAX_PLAYERS; ++player)
        {
            gamepadAxes.emplace_back(static_cast<GamepadAxis>(i), player);
        }
    }
}

void Controls::Update(const sf::Time& Delta)
{
    Input::ProcessInput(this);

    for (const auto& keyboardKey : keyboardKeys)
    {
        if (keyboardKey.state.down)
        {
            BroadcastPressed(keyboardKey.key, PressedInputType::Down, KBM_PLAYER);
        }
        else
        {
            BroadcastPressed(keyboardKey.key, PressedInputType::Up, KBM_PLAYER);
        }
    }
    for (const auto& mouseButton : mouseButtons)
    {
        if (mouseButton.state.down)
        {
            BroadcastPressed(mouseButton.button, PressedInputType::Down, KBM_PLAYER);
        }
        else
        {
            BroadcastPressed(mouseButton.button, PressedInputType::Up, KBM_PLAYER);
        }
    }
    for (const auto& gamepadButton : gamepadButtons)
    {
        if (gamepadButton.state.down)
        {
            BroadcastPressed(gamepadButton.button, PressedInputType::Down, gamepadButton.player);
        }
        else
        {
            BroadcastPressed(gamepadButton.button, PressedInputType::Up, gamepadButton.player);
        }
    }

    for (const auto& mouseAxis : mouseAxes)
    {
        BroadcastAxis(mouseAxis.axis, AxisInputType::Value, mouseAxis.state.value, mouseAxis.state.value, KBM_PLAYER);
    }
    for (const auto & gamepadAxis : gamepadAxes)
    {
        BroadcastAxis(gamepadAxis.axis, AxisInputType::Value, gamepadAxis.state.value, gamepadAxis.state.value, gamepadAxis.player);
    }

    InputViewer::ShowInputViewer();
}

void Controls::LateUpdate(const sf::Time& Delta)
{
    /// Reset all previous frame states

    EnumeratePressedStates([](PressState& PressState)
    {
        PressState.pressed = false;
        PressState.released = false;
    });
}

void Controls::Poll(const sf::Event& Event)
{
    if (Event.is<sf::Event::KeyPressed>())
    {
        PressState state;
        state.down = true;
        SetPressedState(Event.getIf<sf::Event::KeyPressed>()->code, state, KBM_PLAYER);
    }
    else if (Event.is<sf::Event::KeyReleased>())
    {
        SetPressedState(Event.getIf<sf::Event::KeyReleased>()->code, {}, KBM_PLAYER);
    }

    // Mouse & Gamepad polling is done in CaveGameInput
}

void Controls::EnumeratePressedStates(const std::function<void(PressState&)>& callback)
{
    for (auto& key : keyboardKeys)
    {
        callback(key.state);
    }
    for (auto& button : mouseButtons)
    {
        callback(button.state);
    }
    for (auto& button : gamepadButtons)
    {
        callback(button.state);
    }
}

void Controls::BroadcastPressed(const PressedVariant& Pressed, PressedInputType::Type Type, int player)
{
    for (const auto& listener : pressListeners)
    {
        for (const auto& mapping : listener.mappings)
        {
            if (mapping.listenTo != Pressed)
            {
                continue;
            }

            if (!(mapping.type & Type))
            {
                continue;
            }

            if (listener.callback)
            {
                listener.callback(player, Type);
            }
            break;
        }
    }
}

void Controls::BroadcastAxis(const AxisVariant& Axis, AxisInputType::Type Type, float NewValue, float OldValue, int player)
{
    for (const auto& listener : axisListeners)
    {
        for (const auto& mapping : listener.mappings)
        {
            if (mapping.listenTo != Axis)
            {
                continue;
            }

            if (!(mapping.type & Type))
            {
                continue;
            }

            if (listener.callback)
            {
                listener.callback(player, NewValue, OldValue);
            }
            break;
        }
    }
}
