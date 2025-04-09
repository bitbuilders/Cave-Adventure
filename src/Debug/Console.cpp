
#include "Console.h"

#include <imgui-SFML.h>

#include <string>
#include <format>

#include "AssetLibrary.h"
#include "imgui.h"

bool Console::Init(int consoleId, const sf::Vector2u& size, const sf::Vector2i& position)
{
    id = consoleId;

    std::string number = id >= 0 ? std::format(" {}", id) : "";
    std::string name = std::format("Console{}", number);
    window = sf::RenderWindow(sf::VideoMode(size), name);
    window.setPosition(position);

    if (!ImGui::SFML::Init(window, false))
    {
        Shutdown();

        return false;
    }

    AssetLibrary::LoadFonts();

    return true;
}

void Console::Update(const sf::Time& delta)
{
    while (const auto event = window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>())
        {
            Shutdown();
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

    ImGui::Text("Console text");

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

void Console::Shutdown()
{
    if (IsOpen())
    {
        window.close();

        ImGui::SFML::Shutdown(window);

        id = -1;
    }
}

bool Console::IsOpen() const
{
    return window.isOpen();
}

int Console::GetId() const
{
    return id;
}
