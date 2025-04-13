
#include "Console.h"

#include <imgui-SFML.h>

#include <regex>

#include "AssetLibrary.h"
#include "imgui.h"
#include <misc/cpp/imgui_stdlib.h>

#include "Stats.h"


std::vector<std::function<void(const LogLine&)>> Console::onLog;

namespace Log
{
    const std::string File = "saved/logs/log.log";

    const ImVec4 LogColor = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
    const ImVec4 WarningColor = ImVec4(0.9f, 0.95f, 0.05f, 1.0f);
    const ImVec4 ErrorColor = ImVec4(0.9f, 0.15f, 0.1f, 1.0f);

    const ImVec4 StatColor = ImVec4(0.05f, 0.85f, 0.3f, 1.0f);

    const std::string WarningString = "[Warning]";
    const std::string ErrorString = "[Error]";
    const std::string FatalString = "[Fatal Error]";

    const std::string StatString = "[Stat]";

    /** For regex parsing RGBA 0-255 colors in logs (i.e. ##193,25,7,255) */
    const std::string ColorRegex = R"(^#{2}\d\d?\d?,\d\d?\d?,\d\d?\d?,\d\d?\d?)";
}


void Console::EmptyLogFile()
{
    std::fstream log{Log::File, std::fstream::out | std::fstream::trunc};
    log.close();
}

void Console::CleanupLog()
{
    std::fstream oldLog{Log::File};

    std::string contents;

    for (std::string line; std::getline(oldLog, line); )
    {
        const std::regex colorRegex(Log::ColorRegex);
        line = std::regex_replace(line, colorRegex, "");

        contents += line + '\n';
    }

    oldLog.close();

    std::fstream newLog{Log::File, std::fstream::in | std::fstream::out | std::fstream::trunc};

    newLog.write(contents.c_str(), contents.size());

    newLog.close();
}

bool Console::Init(int consoleId, const sf::Vector2u& size, const sf::Vector2i& position)
{
    id = consoleId;

    std::string number = id >= 0 ? std::format(" {}", id + 1) : "";
    std::string name = std::format("Console{}", number);
    window = sf::RenderWindow(sf::VideoMode(size), name);
    window.setPosition(position);

    if (!ImGui::SFML::Init(window, false))
    {
        Close();

        return false;
    }

    AssetLibrary::LoadFonts();

    PullLogsFromFile();

    onLogHandle = static_cast<int>(onLog.size());
    onLog.emplace_back([this](const LogLine& line)
    {
        AddLog(line);
    });

    LOG("Opened {}", name);

    return true;
}

void Console::Update(const sf::Time& delta)
{
    while (const auto event = window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>())
        {
            Close();
        }
    }

    if (!IsOpen())
    {
        return;
    }

    ImGui::SFML::SetCurrentWindow(window);
    ImGui::SFML::Update(window, delta);

    static bool showConsole = true;
    ImGui::SetNextWindowPos({0.0f, 0.0f});
    ImGui::SetNextWindowSize(window.getSize());
    if (!ImGui::Begin("Console", &showConsole, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Tags"))
    {
        ImGui::OpenPopup("Tags");
    }

    static bool showLogs = true;
    static bool showWarnings = true;
    static bool showErrors = true;
    static bool showStats = true;
    if (ImGui::BeginPopup("Tags"))
    {
        ImGui::MenuItem("Logs", "", &showLogs);
        ImGui::MenuItem("Warnings", "", &showWarnings);
        ImGui::MenuItem("Errors", "", &showErrors);
        ImGui::Separator();
        ImGui::MenuItem("Stats", "", &showStats);

        ImGui::EndPopup();
    }

    static std::string filter;
    // ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::SameLine();
    ImGui::InputTextWithHint("##ConsoleFilter", "Search Log", &filter, ImGuiInputTextFlags_None);

    ImGuiTextFilter logFilter(filter.c_str());

    ImGui::SameLine();
    if (ImGui::Button("Clear"))
    {
        Clear();
    }

    ImGui::SameLine();
    static bool autoScroll = true;
    bool autoScrollClicked = false;
    if (ImGui::Checkbox("Auto scroll", &autoScroll))
    {
        autoScrollClicked = autoScroll;
    }

    if (ImGui::BeginChild("ScrollWindow"))
    {
        for (const auto& [message, modifier] : logs)
        {
            if (modifier.options & Stat && !showStats)
            {
                continue;
            }

            if ((modifier.severity == LOG_Log && !showLogs) ||
                (modifier.severity == LOG_Warning && !showWarnings) ||
                (modifier.severity == LOG_Error && !showErrors))
            {
                continue;
            }

            if (!logFilter.PassFilter(message.c_str()))
            {
                continue;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, modifier.color);

            ImGui::Text(message.c_str());

            ImGui::PopStyleColor();
        }

        if (!autoScrollClicked && ImGui::GetScrollY() < ImGui::GetScrollMaxY())
        {
            // For if the user scrolls manually up
            autoScroll = false;
        }
        else if (!wasMaxScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            // For if the user scrolls manually down
            autoScroll = true;
        }

        if (autoScroll)
        {
            ImGui::SetScrollHereY(1.0f);
        }

        wasMaxScroll = ImGui::GetScrollY() >= ImGui::GetScrollMaxY();
    }
    ImGui::EndChild();

    ImGui::End();
}

void Console::Render()
{
    if (!IsOpen())
    {
        return;
    }

    window.clear();

    ImGui::SFML::Render(window);

    window.display();
}

void Console::Close()
{
    if (IsOpen())
    {
        LOG("Closing Console {}", id + 1);

        window.close();

        ImGui::SFML::Shutdown(window);

        id = -1;

        logs.clear();

        if (onLogHandle >= 0)
        {
            onLog.erase(onLog.begin() + onLogHandle);
        }
        onLogHandle = -1;
    }
}

void Console::Shutdown()
{
    Close();
}

void Console::Clear()
{
    logs.clear();
}

bool Console::IsOpen() const
{
    return window.isOpen();
}

int Console::GetId() const
{
    return id;
}

void Console::AddLog(const LogLine& LogLine)
{
    logs.push_back(LogLine);
}

void Console::PullLogsFromFile()
{
    logs.clear();

    std::fstream log{Log::File};

    for (std::string line; std::getline(log, line); )
    {
        LogSeverity severity;
        ParseSeverity(line, severity);

        LogOptions options;
        ParseOptions(line, options);

        ImVec4 lineColor;
        if (!ExtractColor(line, lineColor))
        {
            lineColor = Log::LogColor;
        }

        LogLine logLine;
        logLine.message = line;
        logLine.modifier = { severity, lineColor, options };
        AddLog(logLine);
    }
}

bool Console::ExtractColor(std::string& Log, ImVec4& OutColor)
{
    // ScopedTimer scopedTimer("Console::ExtractColor");

    const std::regex colorRegex(Log::ColorRegex);
    std::smatch colorMatches;
    if (std::regex_search(Log, colorMatches, colorRegex))
    {
        if (!colorMatches.empty())
        {
            std::string color = colorMatches[0].str();

            std::regex digitRegex(R"(\d\d?\d?)");
            std::smatch digitMatches;
            auto searchStart = color.cbegin();

            int parsedDigits = 0;
            while (std::regex_search(searchStart, color.cend(), digitMatches, digitRegex))
            {
                ISTRUE(digitMatches.size() == 1);

                std::string digit = digitMatches[0].str();

                int colorVal = std::stoi(digit);
                float colorPercent = static_cast<float>(colorVal) / 255.0f;

                switch (parsedDigits)
                {
                case 0:
                    OutColor.x = colorPercent;
                    break;
                case 1:
                    OutColor.y = colorPercent;
                    break;
                case 2:
                    OutColor.z = colorPercent;
                    break;
                case 3:
                default:
                    OutColor.w = colorPercent;
                    break;
                }

                searchStart = digitMatches.suffix().first;

                parsedDigits++;
            }

            if (parsedDigits == 4)
            {
                // note: this probably replaces all occurrences
                Log = std::regex_replace(Log, colorRegex, "");

                return true;
            }
        }
    }

    return false;
}

void Console::ParseSeverity(const std::string& Log, LogSeverity& OutSeverity)
{
    OutSeverity = LOG_Log;

    if (Log.contains(Log::WarningString))
    {
        OutSeverity = LOG_Warning;
    }
    else if (Log.contains(Log::ErrorString))
    {
        OutSeverity = LOG_Error;
    }
}

void Console::ParseOptions(const std::string& Log, LogOptions& OutOptions)
{
    OutOptions = None;

    if (Log.contains(Log::StatString))
    {
        OutOptions = static_cast<LogOptions>(OutOptions | Stat);
    }
}
