
#include "InputViewer.h"

#include "imgui.h"

#include "Controls.h"

#include <algorithm>
#include <map>

#include "AssetLibrary.h"
#include "Calc.h"
#include "imgui-SFML.h"


namespace InputViewer
{
    bool showMenu = false;

    constexpr auto showKey = sf::Keyboard::Key::I;

    constexpr float buttonSize = 30.0f;

    constexpr float joystickSize = 20.0f;
    constexpr float joystickHoleSize = 40.0f;

    const std::map<sf::Keyboard::Key, std::string> keyStrings =
        {
            {sf::Keyboard::Key::Unknown,     "Unknown"   },
            {sf::Keyboard::Key::A,           "A"         },
            {sf::Keyboard::Key::B,           "B"         },
            {sf::Keyboard::Key::C,           "C"         },
            {sf::Keyboard::Key::D,           "D"         },
            {sf::Keyboard::Key::E,           "E"         },
            {sf::Keyboard::Key::F,           "F"         },
            {sf::Keyboard::Key::G,           "G"         },
            {sf::Keyboard::Key::H,           "H"         },
            {sf::Keyboard::Key::I,           "I"         },
            {sf::Keyboard::Key::J,           "J"         },
            {sf::Keyboard::Key::K,           "K"         },
            {sf::Keyboard::Key::L,           "L"         },
            {sf::Keyboard::Key::M,           "M"         },
            {sf::Keyboard::Key::N,           "N"         },
            {sf::Keyboard::Key::O,           "O"         },
            {sf::Keyboard::Key::P,           "P"         },
            {sf::Keyboard::Key::Q,           "Q"         },
            {sf::Keyboard::Key::R,           "R"         },
            {sf::Keyboard::Key::S,           "S"         },
            {sf::Keyboard::Key::T,           "T"         },
            {sf::Keyboard::Key::U,           "U"         },
            {sf::Keyboard::Key::V,           "V"         },
            {sf::Keyboard::Key::W,           "W"         },
            {sf::Keyboard::Key::X,           "X"         },
            {sf::Keyboard::Key::Y,           "Y"         },
            {sf::Keyboard::Key::Z,           "Z"         },
            {sf::Keyboard::Key::Num0,        "Num0"      },
            {sf::Keyboard::Key::Num1,        "Num1"      },
            {sf::Keyboard::Key::Num2,        "Num2"      },
            {sf::Keyboard::Key::Num3,        "Num3"      },
            {sf::Keyboard::Key::Num4,        "Num4"      },
            {sf::Keyboard::Key::Num5,        "Num5"      },
            {sf::Keyboard::Key::Num6,        "Num6"      },
            {sf::Keyboard::Key::Num7,        "Num7"      },
            {sf::Keyboard::Key::Num8,        "Num8"      },
            {sf::Keyboard::Key::Num9,        "Num9"      },
            {sf::Keyboard::Key::Escape,      "Escape    " },
            {sf::Keyboard::Key::LControl,    "LControl  " },
            {sf::Keyboard::Key::LShift,      "LShift    "  },
            {sf::Keyboard::Key::LAlt,        "LAlt      " },
            {sf::Keyboard::Key::LSystem,     "LSystem   " },
            {sf::Keyboard::Key::RControl,    "RControl  " },
            {sf::Keyboard::Key::RShift,      "RShift    " },
            {sf::Keyboard::Key::RAlt,        "RAlt      " },
            {sf::Keyboard::Key::RSystem,     "RSystem   " },
            {sf::Keyboard::Key::Menu,        "Menu      " },
            {sf::Keyboard::Key::LBracket,    "LBracket  " },
            {sf::Keyboard::Key::RBracket,    "RBracket  " },
            {sf::Keyboard::Key::Semicolon,   "Semicolon " },
            {sf::Keyboard::Key::Comma,       "Comma     " },
            {sf::Keyboard::Key::Period,      "Period    " },
            {sf::Keyboard::Key::Apostrophe,  "Apostrophe" },
            {sf::Keyboard::Key::Slash,       "Slash     " },
            {sf::Keyboard::Key::Backslash,   "Backslash " },
            {sf::Keyboard::Key::Grave,       "Grave     " },
            {sf::Keyboard::Key::Equal,       "Equal     " },
            {sf::Keyboard::Key::Hyphen,      "Hyphen    " },
            {sf::Keyboard::Key::Space,       "Space     " },
            {sf::Keyboard::Key::Enter,       "Enter     " },
            {sf::Keyboard::Key::Backspace,   "Backspace " },
            {sf::Keyboard::Key::Tab,         "Tab       " },
            {sf::Keyboard::Key::PageUp,      "PageUp    " },
            {sf::Keyboard::Key::PageDown,    "PageDown  " },
            {sf::Keyboard::Key::End,         "End       " },
            {sf::Keyboard::Key::Home,        "Home      " },
            {sf::Keyboard::Key::Insert,      "Insert    " },
            {sf::Keyboard::Key::Delete,      "Delete    " },
            {sf::Keyboard::Key::Add,         "Add       " },
            {sf::Keyboard::Key::Subtract,    "Subtract  " },
            {sf::Keyboard::Key::Multiply,    "Multiply  " },
            {sf::Keyboard::Key::Divide,      "Divide    " },
            {sf::Keyboard::Key::Left,        "Left      " },
            {sf::Keyboard::Key::Right,       "Right     " },
            {sf::Keyboard::Key::Up,          "Up        " },
            {sf::Keyboard::Key::Down,        "Down      " },
            {sf::Keyboard::Key::Numpad0,     "Numpad0   " },
            {sf::Keyboard::Key::Numpad1,     "Numpad1   " },
            {sf::Keyboard::Key::Numpad2,     "Numpad2   " },
            {sf::Keyboard::Key::Numpad3,     "Numpad3   " },
            {sf::Keyboard::Key::Numpad4,     "Numpad4   " },
            {sf::Keyboard::Key::Numpad5,     "Numpad5   " },
            {sf::Keyboard::Key::Numpad6,     "Numpad6   " },
            {sf::Keyboard::Key::Numpad7,     "Numpad7   " },
            {sf::Keyboard::Key::Numpad8,     "Numpad8   " },
            {sf::Keyboard::Key::Numpad9,     "Numpad9   " },
            {sf::Keyboard::Key::F1,          "F1        " },
            {sf::Keyboard::Key::F2,          "F2        " },
            {sf::Keyboard::Key::F3,          "F3        " },
            {sf::Keyboard::Key::F4,          "F4        " },
            {sf::Keyboard::Key::F5,          "F5        " },
            {sf::Keyboard::Key::F6,          "F6        " },
            {sf::Keyboard::Key::F7,          "F7        " },
            {sf::Keyboard::Key::F8,          "F8        " },
            {sf::Keyboard::Key::F9,          "F9        " },
            {sf::Keyboard::Key::F10,         "F10       " },
            {sf::Keyboard::Key::F11,         "F11       " },
            {sf::Keyboard::Key::F12,         "F12       " },
            {sf::Keyboard::Key::F13,         "F13       " },
            {sf::Keyboard::Key::F14,         "F14       " },
            {sf::Keyboard::Key::F15,         "F15       " },
            {sf::Keyboard::Key::Pause,       "Pause     " }
        };
}


void InputViewer::ShowInputViewer()
{
#if !DEBUG
    return;
#endif

    Controls& controls = Controls::Get();

    bool lsDown = controls.GetPressedState(GamepadButton::LS, PressedInputType::Down);
    bool yDown = controls.GetPressedState(GamepadButton::North, PressedInputType::Down);
    if (Controls::Get().GetPressedState(showKey) || (lsDown && yDown))
    {
        showMenu = true;
        ImGui::SetWindowFocus("Input Viewer");
    }

    if (!showMenu)
    {
        return;
    }

    ImGui::SetNextWindowPos({200, 100}, ImGuiCond_Once);
    ImGui::SetNextWindowSize({400, 600}, ImGuiCond_Once);
    if (!ImGui::Begin("Input Viewer", &showMenu, ImGuiWindowFlags_NoNav))
    {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, {0.5f, 0.5f});

    ImGui::PushFont(Fonts::FontAwesome::LetterFont);

    ImGui::BeginGroup();
    ImGui::Indent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Selectable("W", controls.GetPressedState(sf::Keyboard::Key::W, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::Unindent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Selectable("A", controls.GetPressedState(sf::Keyboard::Key::A, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::SameLine();
    ImGui::Selectable("S", controls.GetPressedState(sf::Keyboard::Key::S, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::SameLine();
    ImGui::Selectable("D", controls.GetPressedState(sf::Keyboard::Key::D, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::EndGroup();

    ImGui::PopFont();

    std::vector<sf::Keyboard::Key> downKeys;
    controls.GetDownKeys(downKeys);

    std::string keysString;
    for (const auto& key : downKeys)
    {
        std::string str = keyStrings.at(key);
        auto remove = std::ranges::remove(str, ' ');
        str.erase(remove.begin(), remove.end());
        keysString += str + ", ";
    }

    if (keysString.size() >= 2)
    {
        keysString.replace(keysString.size() - 2, 2, "");
    }
    ImGui::TextWrapped("Down Keys: %s", keysString.c_str());

    ImGui::SeparatorText("Mouse");

    ImGui::PushFont(Fonts::FontAwesome::LetterFont);
    ImGui::BeginGroup();
    ImGui::Selectable("LMB", controls.GetPressedState(sf::Mouse::Button::Left, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize * 1.5f, buttonSize});
    ImGui::SameLine();
    ImGui::Selectable("MMB", controls.GetPressedState(sf::Mouse::Button::Middle, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize * 1.5f, buttonSize});
    ImGui::SameLine();
    ImGui::Selectable("RMB", controls.GetPressedState(sf::Mouse::Button::Right, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize * 1.5f, buttonSize});
    ImGui::Selectable("MB5", controls.GetPressedState(sf::Mouse::Button::Extra2, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize * 1.5f, buttonSize});
    ImGui::SameLine();
    ImGui::Selectable("MB4", controls.GetPressedState(sf::Mouse::Button::Extra1, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize * 1.5f, buttonSize});
    ImGui::EndGroup();
    ImGui::PopFont();

    ImGui::SameLine();

    const auto mousePadSize = buttonSize * 2.0f;
    ImVec2 mousepadStart = ImGui::GetCursorPos();
    ImGui::Button("##mousepad", {mousePadSize, mousePadSize});
    ImVec2 mousepadStartDelta = {mousepadStart.x - ImGui::GetCursorPos().x, mousepadStart.y - ImGui::GetCursorPos().y};
    ImVec2 mouseCenter = {mousepadStartDelta.x + mousePadSize * 0.5f, mousepadStartDelta.y + mousePadSize * 0.5f};

    static std::vector<sf::Vector2f> mouseDeltas(20);
    static int nextMouseDelta = 0;

    mouseDeltas[nextMouseDelta] =
        {
        Math::Clamp(controls.GetAxis(MouseAxis::X) * 3.0f, -mousePadSize * 0.5f, mousePadSize * 0.5f),
        Math::Clamp(controls.GetAxis(MouseAxis::Y) * 3.0f, -mousePadSize * 0.5f, mousePadSize * 0.5f)
        };
    nextMouseDelta = (nextMouseDelta + 1) % static_cast<int>(mouseDeltas.size());
    auto mouseDeltaX = 0.0f;
    auto mouseDeltaY = 0.0f;
    for (auto mouseDelta : mouseDeltas)
    {
        mouseDeltaX += mouseDelta.x;
        mouseDeltaY += mouseDelta.y;
    }
    mouseDeltaX /= static_cast<float>(mouseDeltas.size());
    mouseDeltaY /= static_cast<float>(mouseDeltas.size());

    ImVec2 mouseTarget = {mouseCenter.x + mouseDeltaX, mouseCenter.y + mouseDeltaY};
    ImGui::DrawLine(mouseCenter, mouseTarget, sf::Color(250, 190, 40), 3.0f);

    static std::vector<float> scrollDeltas(50);
    static int nextScrollDelta = 0;

    scrollDeltas[nextScrollDelta] = controls.GetAxis(MouseAxis::Wheel) * 10.0f;
    nextScrollDelta = (nextScrollDelta + 1) % static_cast<int>(scrollDeltas.size());

    auto wheelDelta = 0.0f;
    for (auto scrollDelta : scrollDeltas)
    {
        wheelDelta += scrollDelta;
    }
    wheelDelta /= static_cast<float>(scrollDeltas.size());

    constexpr auto scrollScale = 1.0f;
    wheelDelta *= scrollScale;

    ImGui::VSliderFloat("scroll", {buttonSize * 1.5f, buttonSize * 2.0f}, &wheelDelta, -scrollScale, scrollScale, "%.1f");

    ImGui::SeparatorText("Gamepad");

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100.0f);


    ImGui::BeginGroup();

    ImGui::Indent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);

    ImGui::PushFont(Fonts::FontAwesome::LetterFont);

    float lt = controls.GetAxis(GamepadAxis::LT);
    ImGui::ProgressBar(lt, {buttonSize * 2.0f, buttonSize}, "LT");
    ImGui::Selectable("LB", controls.GetPressedState(GamepadButton::LB, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize * 2.0f, buttonSize});

    ImGui::Unindent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);

    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::BeginGroup();

    ImGui::Indent((buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f) * 3.0f);

    float rt = controls.GetAxis(GamepadAxis::RT);
    ImGui::ProgressBar(rt, {buttonSize * 2.0f, buttonSize}, "RT");
    ImGui::Selectable("RB", controls.GetPressedState(GamepadButton::RB, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize * 2.0f, buttonSize});

    ImGui::PopFont();

    ImGui::Unindent((buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f) * 3.0f);

    ImGui::EndGroup();


    ImGui::BeginGroup();
    ImGui::Indent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Selectable(ICON_FA_CIRCLE_UP, controls.GetPressedState(GamepadButton::Up, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::Unindent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Selectable(ICON_FA_CIRCLE_LEFT, controls.GetPressedState(GamepadButton::Left, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::SameLine(0, buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    // ImGui::Indent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Selectable(ICON_FA_CIRCLE_RIGHT, controls.GetPressedState(GamepadButton::Right, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    // ImGui::Unindent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Indent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Selectable(ICON_FA_CIRCLE_DOWN, controls.GetPressedState(GamepadButton::Down, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::Unindent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::EndGroup();
    ImGui::SameLine();

    auto logo = controls.GetGamepadType(KBM_PLAYER) == GamepadType::PS5 ? ICON_FA_PLAYSTATION : ICON_FA_XBOX;

    ImGui::Selectable(ICON_FA_CIRCLE_DOT, controls.GetPressedState(GamepadButton::Special, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::SameLine();
    ImGui::Selectable(logo, controls.GetPressedState(GamepadButton::Logo, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::SameLine();
    ImGui::Selectable(ICON_FA_GRIP_LINES, controls.GetPressedState(GamepadButton::Menu, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::SameLine();

    ImGui::PushFont(Fonts::FontAwesome::LetterFont);

    ImGui::BeginGroup();
    ImGui::Indent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Selectable(ICON_FA_Y, controls.GetPressedState(GamepadButton::North, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::Unindent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Selectable(ICON_FA_X, controls.GetPressedState(GamepadButton::West, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::SameLine(0, buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Selectable(ICON_FA_B, controls.GetPressedState(GamepadButton::East, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::Indent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::Selectable(ICON_FA_A, controls.GetPressedState(GamepadButton::South, PressedInputType::Down), ImGuiSelectableFlags_None, {buttonSize, buttonSize});
    ImGui::Unindent(buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f);
    ImGui::EndGroup();

    ImGui::PopFont();

    ImGui::Indent((buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f) * 2.0f);

    float joystickSizeDelta = joystickHoleSize - joystickSize * 1.5f;
    sf::Vector2f leftOffset = {controls.GetAxis(GamepadAxis::LSX), -controls.GetAxis(GamepadAxis::LSY)};
    leftOffset = leftOffset* joystickSizeDelta;
    ImVec2 leftHolePos = ImGui::GetCursorScreenPos();
    ImVec2 leftHoleCenter = {leftHolePos.x + joystickHoleSize * 0.5f + leftOffset.x, leftHolePos.y + joystickHoleSize * 0.5f + leftOffset.y};
    ImVec2 leftStickPos = {leftHoleCenter.x - joystickSize * 0.5f, leftHoleCenter.y - joystickSize * 0.5f};
    ImGui::Button("##leftHole", {joystickHoleSize, joystickHoleSize});

    ImGui::SameLine();
    ImGui::Indent((buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f) * 3.0f);
    sf::Vector2f rightOffset = {controls.GetAxis(GamepadAxis::RSX), -controls.GetAxis(GamepadAxis::RSY)};
    rightOffset = rightOffset * joystickSizeDelta;
    ImVec2 rightHolePos = ImGui::GetCursorScreenPos();
    ImVec2 rightHoleCenter = {rightHolePos.x + joystickHoleSize * 0.5f + rightOffset.x, rightHolePos.y + joystickHoleSize * 0.5f + rightOffset.y};
    ImVec2 rightStickPos = {rightHoleCenter.x - joystickSize * 0.5f, rightHoleCenter.y - joystickSize * 0.5f};
    ImGui::Button("##rightHole", {joystickHoleSize, joystickHoleSize});

    ImGui::Unindent((buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f) * 3.0f);

    if (lsDown)
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogram));
    ImGui::SetCursorScreenPos(leftStickPos);
    ImGui::Button("##leftJoystick", {joystickSize, joystickSize});
    if (lsDown)
        ImGui::PopStyleColor();


    bool rsDown = controls.GetPressedState(GamepadButton::RS, PressedInputType::Down);
    if (rsDown)
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogram));
    ImGui::SetCursorScreenPos(rightStickPos);
    ImGui::Button("##rightJoystick", {joystickSize, joystickSize});
    if (rsDown)
        ImGui::PopStyleColor();

    ImGui::Unindent((buttonSize + ImGui::GetStyle().FramePadding.x * 2.0f) * 2.0f);

    ImGui::PopStyleVar(); // ImGuiStyleVar_FrameRounding

    ImGui::PopStyleVar(); // ImGuiStyleVar_SelectableTextAlign

    ImGui::End();
}
