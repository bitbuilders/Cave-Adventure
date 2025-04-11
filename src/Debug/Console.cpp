
#include "Console.h"

#include <imgui-SFML.h>

#include <format>
#include <filesystem>

#include "AssetLibrary.h"
#include "imgui.h"


namespace Log
{
    const std::string File = "saved/logs/log.log";
}


void Console::Log(const std::string& Msg, const LogModifier& Modifier, LogOptions Options)
{
    std::string directory = Log::File.substr(0, Log::File.rfind('/'));
    std::filesystem::create_directories(directory);

    std::fstream log{Log::File, std::fstream::in | std::fstream::out | std::fstream::app};

    log << Msg << std::endl;

    log.close();
}

void Console::Log(const std::string& Msg)
{
    Log(Msg, LogModifier(), None);
}

void Console::LogWarning(const std::string& Msg)
{
    Log(Msg, { ImVec4(0.85f, 0.95f, 0.05f, 1.0f) }, Warning);
}

void Console::LogError(const std::string& Msg)
{
    Log(Msg, { ImVec4(0.9f, 0.2f, 0.1f, 1.0f) }, Error);
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

    Log("Hello");
    Log("World");
    Log("ImGui!");

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

        std::string line;
        while (log.getline(line.data(), sizeof(std::string)))
        {
            ImGui::Text(line.c_str());
        }

        log.close();
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
