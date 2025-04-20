
#pragma once

#include "Module.h"

#include <SFML/Window/Mouse.hpp>

#include <functional>
#include <map>
#include <vector>
#include <variant>

#include "Controllers/Gamepad.h"
#include "Controllers/Keyboard.h"
#include "Controllers/Mouse.h"


/** The keyboard and mouse player index */
#define KBM_PLAYER 0

/** The total number of local players */
#define MAX_PLAYERS 4

using CallbackHandle = uint32_t;

enum class GamepadMode
{
    Xbox,
    PS5,
    Custom
};

enum class MouseAxis
{
    X, // Horizontal movement
    Y, // Vertical movement
    Wheel, // Scroll wheel

    COUNT
};

enum class GamepadAxis
{
    LSX, // Left stick horizontal
    LSY, // Left stick vertical
    RSX, // Right stick horizontal
    RSY, // Right stick vertical
    LT, // Left trigger
    RT, // Right trigger

    COUNT
};

enum class GamepadButton
{
    South, // Bottom face button
    East, // Right face button
    North, // Top face button
    West, // Left face button
    Down, // D-pad down
    Right, // D-pad right
    Up, // D-pad up
    Left, // D-pad left
    RB, // Right bumper
    LB, // Left bumper
    RS, // Right stick
    LS, // Left stick
    Menu, // Menu/start
    Special, // The button that's not Menu
    Logo, // Xbox/Playstation

    COUNT
};

enum class PressedInputType
{
    Pressed = 0,
    Released = 1 << 0,
    Down = 1 << 1,
    Up = 1 << 2
};

enum class AxisInputType
{
    ValueChanged = 0,
    Value = 1 << 0
};

struct PressState
{
    bool pressed = false;
    bool released = false;
    bool down = false;
};

struct AxisState
{
    float value = 0.0f;
};

struct KeyboardKeyState
{
    sf::Keyboard::Key key = sf::Keyboard::Key::A;

    PressState state;
};

struct MouseButtonState
{
    sf::Mouse::Button button = sf::Mouse::Button::Left;

    PressState state;
};

struct MouseAxisState
{
    MouseAxis axis = MouseAxis::X;

    AxisState state;
};

struct GamepadButtonState
{
    GamepadButton button = GamepadButton::South;

    int player = 0;

    PressState state;
};

struct GamepadAxisState
{
    GamepadAxis axis = GamepadAxis::LSX;

    int player = 0;

    AxisState state;
};

using PressedVariant = std::variant<sf::Mouse::Button, sf::Keyboard::Key, GamepadButton>;

struct PressCallback
{
    PressedVariant listenTo;

    PressedInputType type = PressedInputType::Pressed;

    std::function<void(int /*Player*/)> callback;

private:
    CallbackHandle handle = 0;

    friend class Controls;
};

using AxisVariant = std::variant<MouseAxis, GamepadAxis>;

struct AxisCallback
{
    AxisVariant listenTo;

    AxisInputType type = AxisInputType::ValueChanged;

    std::function<void(int /*Player*/,float /*NewValue*/, float /*OldValue*/)> callback;

private:
    CallbackHandle handle = -1;

    friend class Controls;
};

class Controls final : public Module
{
public:
    static Controls& Get();

private:
    std::vector<PressCallback> pressListeners;
    std::vector<AxisCallback> axisListeners;

    std::vector<KeyboardKeyState> keyboardKeys;
    std::vector<MouseButtonState> mouseButtons;
    std::vector<GamepadButtonState> gamepadButtons;

    std::vector<MouseAxisState> mouseAxes;
    std::vector<GamepadAxisState> gamepadAxes;

    Keyboard keyboard;
    Mouse mouse;
    std::map<int, std::shared_ptr<Gamepad>> gamepads;

public:
    CallbackHandle ListenForPress(PressCallback& Callback);

    CallbackHandle ListenForAxis(AxisCallback& Callback);

    void RemoveListener(CallbackHandle Handle);

    /** Get the value of a button/key immediately (consider using ListenForPress instead of repeated calls to this) */
    [[nodiscard]] bool GetPressedState(const PressedVariant& pressed, PressedInputType type = PressedInputType::Pressed, int player = 0) const;

    /** Get the value of an axis immediately (consider using ListenForAxis instead of repeated calls to this) */
    [[nodiscard]] float GetAxis(const AxisVariant& axis, int player = 0) const;

    void SetPressedState(const PressedVariant& Pressed, const PressState& State, int player = 0);

    void SetAxisState(const AxisVariant& Axis, const AxisState& State, int player = 0);

    void SetGamepadMode(GamepadMode mode, int player = 0);

public:
    Controls();
    ~Controls() override = default;

    /// Module interface start
    constexpr std::string GetName() override { return "Controls"; }
    /// Module interface end

    /// Tickable interface start
    void LateUpdate(const sf::Time& Delta) override;

    void Poll(const sf::Event& Event) override;

    constexpr bool CanUpdate() override { return false; }
    constexpr bool CanLateUpdate() override { return true; }
    constexpr bool CanPoll() override { return true; }
    /// Tickable interface end

private:
    void EnumeratePressedStates(const std::function<void(PressState&)>& callback);

private:
    static CallbackHandle currentHandle;
};
