
#pragma once

#include <SFML/Graphics.hpp>

#include "imgui.h"

#include <string>
#include <fstream>
#include <iostream>

namespace Log
{
    extern const std::string File;
}

enum LogOptions
{
    None = 0,
    Warning = 1 << 0,
    Error = 1 << 1,

    Timestamp = 1 << 2
};

struct LogModifier
{
    ImVec4 color;
};

class Console
{
private:
    sf::RenderWindow window;

    int id = -1;

public:
    static void Log(const std::string& Msg, const LogModifier& Modifier, LogOptions Options);

    static void Log(const std::string& Msg);

    static void LogWarning(const std::string& Msg);

    static void LogError(const std::string& Msg);

    static void ClearLog();

public:
    bool Init(int consoleId = -1, const sf::Vector2u& size = {600u, 400u}, const sf::Vector2i& position = {0, 0});

    void Update(const sf::Time& delta);

    void Render();

    void Close();

    void Shutdown();

    bool IsOpen() const;

    int GetId() const;
};
