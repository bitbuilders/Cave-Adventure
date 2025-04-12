
#pragma once

#include <SFML/Graphics.hpp>

#include "imgui.h"

#include <string>
#include <format>

#include "DebugUtils.h"


/**
 * Print a log to the console (supports std::format style arguments!)
 * @param Msg The log message
 */
#define LOG(Msg, ...) Console::Log(Msg, { Log::LogColor }, __VA_ARGS__);

/**
 * Print a modified log to the console
 * @param Msg The log message
 * @param Mod The log modifier (i.e. color, severity)
 */
#define LOG_M(Msg, Mod, ...) Console::Log(Msg, Mod, __VA_ARGS__);

/**
 * Print a log to the console with the line/function it was called from prepended
 * @param Msg The log message
 */
#define LOG_L(Msg, ...) LOG(std::format("{}: {}", DebugUtils::GetCallLine(), Msg), __VA_ARGS__)

/**
 * Print a modified log to the console with the line/function it was called from prepended
 * @param Msg The log message
 * @param Mod The log modifier (i.e. color, severity)
 */
#define LOG_LM(Msg, Mod, ...) LOG_M(std::format("{}: {}", DebugUtils::GetCallLine(), Msg), Mod, __VA_ARGS__)

/**
 * Print a warning to the console
 * @param Msg The warning message (will be yellow)
 */
#define LOG_WARN(Msg, ...) LOG_M(Msg, { Log::WarningColor, Warning }, __VA_ARGS__)

/**
 * Print an error message to the console
 * @param Msg The error message (will be red)
 */
#define LOG_ERROR(Msg, ...) LOG_M(Msg, { Log::ErrorColor, Error }, __VA_ARGS__)

/**
 * Print a fatal error message to the console then teardown the application (need to check log file after for the message)
 * @param Msg The fatal error message
 */
#define LOG_FATAL(Msg, ...) LOG_M(std::format("{} -> {}", Msg, DebugUtils::GetCallLine()), { Log::ErrorColor, Fatal }, __VA_ARGS__)

namespace Log
{
    extern const std::string File;

    extern const ImVec4 LogColor;
    extern const ImVec4 WarningColor;
    extern const ImVec4 ErrorColor;
}

enum LogOptions
{
    None = 0,
    Warning = 1 << 0,
    Error = 1 << 1,
    Fatal = 1 << 2,

    Timestamp = 1 << 3
};

struct LogModifier
{
    ImVec4 color;

    LogOptions options = None;
};

class Console
{
private:
    sf::RenderWindow window;

    int id = -1;

public:
    template<typename... Types>
    static void Log(const std::string& Msg, const LogModifier& Modifier, Types... Args);

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
