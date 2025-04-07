#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "Game/Game.h"

int main(int argc, char* argv[])
{
    auto window = sf::RenderWindow(
        sf::VideoMode({1920u, 1080u}),
        "Adventure",
        sf::Style::Default,
        sf::State::Windowed);
    window.setFramerateLimit(0);
    if (!ImGui::SFML::Init(window))
    {
        return 1;
    }

    Game game;
    game.Init(window);

    sf::Clock clock;
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        sf::Time delta = clock.restart();
        game.Update(window, delta);
        ImGui::SFML::Update(window, delta);

        ImGui::ShowDemoWindow();

        window.clear();

        game.Render(window);
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
}
