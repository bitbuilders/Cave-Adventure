
#pragma once

#include <SFML/Graphics.hpp>

#include "imgui.h"

#include <string>
#include <format>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <cmath>
#include <functional>
#include <vector>

#include "DebugUtils.h"


#pragma region LogMacros

/**
 * Print a log to the console (supports std::format style arguments!)
 * @param Msg The log message
 */
#define LOG(Msg, ...) Console::Log(Msg, { LOG_Log, Log::LogColor }, ##__VA_ARGS__);

/**
 * Print a modified log to the console
 * @param Msg The log message
 * @param Mod The log modifier (i.e. color, severity)
 */
#define LOG_M(Msg, Mod, ...) Console::Log(Msg, Mod, ##__VA_ARGS__);

/**
 * Print a log to the console with the line/function it was called from prepended
 * @param Msg The log message
 */
#define LOG_L(Msg, ...) LOG(std::format("{}: {}", DebugUtils::GetCallLine(), Msg), ##__VA_ARGS__)

/**
 * Print a modified log to the console with the line/function it was called from prepended
 * @param Msg The log message
 * @param Mod The log modifier (i.e. color, severity)
 */
#define LOG_LM(Msg, Mod, ...) LOG_M(std::format("{}: {}", DebugUtils::GetCallLine(), Msg), Mod, ##__VA_ARGS__)

/**
 * Print a modified log to the console with the line/function it was called from prepended
 * @param Msg The log message
 * @param Mod The log modifier (i.e. color, severity)
 */
#define LOG_STAT(Msg, ...) LOG_M(Msg, { LOG_Log, Log::StatColor, Stat }, ##__VA_ARGS__)

/**
 * Print a warning to the console
 * @param Msg The warning message (will be yellow)
 */
#define LOG_WARN(Msg, ...) LOG_M(Msg, { LOG_Warning, Log::WarningColor }, ##__VA_ARGS__)

/**
 * Print an error message to the console
 * @param Msg The error message (will be red)
 */
#define LOG_ERROR(Msg, ...) LOG_M(Msg, { LOG_Error, Log::ErrorColor }, ##__VA_ARGS__)

/**
 * Print a fatal error message to the console then teardown the application (need to check log file after for the message)
 * @param Msg The fatal error message
 */
#define LOG_FATAL(Msg, ...) LOG_M(std::format("{} -> {}", Msg, DebugUtils::GetCallLine()), { LOG_Fatal, Log::ErrorColor }, ##__VA_ARGS__)

#pragma endregion LogMacros


namespace Log
{
    extern const std::string File;

    extern const ImVec4 LogColor;
    extern const ImVec4 WarningColor;
    extern const ImVec4 ErrorColor;

    extern const ImVec4 StatColor;

    extern const std::string WarningString;
    extern const std::string ErrorString;
    extern const std::string FatalString;

    extern const std::string StatString;
}

enum LogOptions
{
    None = 0,
    Timestamp = 1 << 0,
    Stat = 1 << 1
};

enum LogSeverity
{
    LOG_Log,
    LOG_Warning,
    LOG_Error,
    LOG_Fatal
};

struct LogModifier
{
    LogSeverity severity = LOG_Log;

    ImVec4 color;

    LogOptions options = None;
};

// internal
struct LogLine
{
    std::string message;

    LogModifier modifier;
};

typedef int OnLogHandle;

class Console
{
private:
    sf::RenderWindow window;

    int id = -1;

    OnLogHandle onLogHandle = -1;

    /** the logs for this console while it is open */
    std::vector<LogLine> logs;

public:
    template<typename... Types>
    static void Log(const std::string& Msg, const LogModifier& Modifier, Types&&... Args);

    /** Empties the contents of the log file (usually done at game initialization) */
    static void EmptyLogFile();

    /**
     * Removes things like colors or other formatting from the log to make it more readable outside of runtime
     */
    static void CleanupLog();

private:
    static std::vector<std::function<void(const LogLine&)>> onLog;

public:
    bool Init(int consoleId = -1, const sf::Vector2u& size = {600u, 400u}, const sf::Vector2i& position = {0, 0});

    void Update(const sf::Time& delta);

    void Render();

    void Close();

    void Shutdown();

    void Clear();

    bool IsOpen() const;

    int GetId() const;

private:
    void AddLog(const LogLine& LogLine);

    void PullLogsFromFile();

    bool ExtractColor(std::string& Log, ImVec4& OutColor);

    void ParseSeverity(const std::string& Log, LogSeverity& OutSeverity);

    void ParseOptions(const std::string& Log, LogOptions& OutOptions);

private:
    bool wasMaxScroll = false;
};

template <typename ... Types>
void Console::Log(const std::string& Msg, const LogModifier& Modifier, Types&&... Args)
{
    std::string directory = Log::File.substr(0, Log::File.rfind('/'));
    std::filesystem::create_directories(directory);

    std::fstream log{Log::File, std::fstream::in | std::fstream::app};

    std::string mods;

    std::string color = std::format("##{},{},{},{}",
        static_cast<int>(std::floor(Modifier.color.x * 255.0f)),
        static_cast<int>(std::floor(Modifier.color.y * 255.0f)),
        static_cast<int>(std::floor(Modifier.color.z * 255.0f)),
        static_cast<int>(std::floor(Modifier.color.w * 255.0f)));
    mods += color;

#if !__APPLE__
    if (Modifier.options & Timestamp)
    {
        const auto utc = std::chrono::system_clock::now();
        auto local = std::chrono::current_zone()->to_local(utc);
        mods += std::format("[{:%m-%d-%Y %I:%M:%S %p}] ", std::chrono::time_point_cast<std::chrono::milliseconds>(local));
    }
#endif

    std::string severity;
    if (Modifier.severity == LOG_Fatal)
    {
        severity = Log::FatalString;
    }
    else if (Modifier.severity == LOG_Error)
    {
        severity = Log::ErrorString;
    }
    else if (Modifier.severity == LOG_Warning)
    {
        severity = Log::WarningString;
    }

    if (!severity.empty())
    {
        mods = std::format("{}{} ", mods, severity);
    }

    if (Modifier.options & Stat)
    {
        mods = std::format("{}{} ", mods, Log::StatString);
    }

    std::string fmt = std::format("{}{}", mods, Msg);
    std::string formattedMsg = std::vformat(fmt, std::make_format_args(Args...));

    log << formattedMsg << std::endl;

    log.close();

    for (const auto& callback : onLog)
    {
        if (callback)
        {
            LogLine line;
            line.message = formattedMsg.replace(0, color.length(), "");
            line.modifier = Modifier;
            callback(line);
        }
    }

    if (Modifier.severity == LOG_Fatal)
    {
        CHECK(false, "Fatal error (see logs)");
    }
}
