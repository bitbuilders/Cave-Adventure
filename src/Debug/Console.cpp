
#include "Console.h"

#include <imgui-SFML.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "AssetLibrary.h"
#include "imgui.h"


namespace Log
{
    const std::string File = "saved/logs/log.log";

    const ImVec4 LogColor = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
    const ImVec4 WarningColor = ImVec4(0.9f, 0.95f, 0.05f, 1.0f);
    const ImVec4 ErrorColor = ImVec4(0.9f, 0.15f, 0.1f, 1.0f);
}


template<typename... Types>
void Console::Log(const std::string& Msg, const LogModifier& Modifier, Types... Args)
{
    std::string directory = Log::File.substr(0, Log::File.rfind('/'));
    std::filesystem::create_directories(directory);

    std::fstream log{Log::File, std::fstream::in | std::fstream::app};

    std::string formattedMsg = std::vformat(Msg, std::make_format_args(Args...));

    log << formattedMsg << std::endl;

    log.close();

    if (Modifier.options & Fatal)
    {
        CHECK(false, "Fatal error (see logs)");
    }
}

void Console::ClearLog()
{
    std::fstream log{Log::File, std::fstream::out | std::fstream::trunc};
    log.close();
}

bool Console::Init(int consoleId, const sf::Vector2u& size, const sf::Vector2i& position)
{
    id = consoleId;

    std::string number = id >= 0 ? std::format(" {}", id) : "";
    std::string name = std::format("Console{}", number);
    window = sf::RenderWindow(sf::VideoMode(size), name);
    window.setPosition(position);

    if (!ImGui::SFML::Init(window, false))
    {
        Close();

        return false;
    }

    AssetLibrary::LoadFonts();

    Log("Hello", {});
    Log("World", LogModifier());
    Log("ImGui!", LogModifier());
    Log("Number {}", LogModifier(), 2);
    Log("Float {:.2f}", LogModifier(), 5.1263f);
    LOG("Macro");
    LOG("Macro {} two!", "is number");

    LOG_L("says hi!");

    // LOG_FATAL("Oops");

    for (int i = 0; i < 100; ++i)
    {
        LOG("It is {}", i);
    }

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
    if (!ImGui::Begin("Console", &showConsole))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginChild("ScrollWindow"))
    {
        std::fstream log{Log::File};

        for (std::string line; std::getline(log, line); )
        {
            ImGui::Text(line.c_str());
        }
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
        window.close();

        ImGui::SFML::Shutdown(window);

        id = -1;
    }
}

void Console::Shutdown()
{
    Close();
}

bool Console::IsOpen() const
{
    return window.isOpen();
}

int Console::GetId() const
{
    return id;
}
