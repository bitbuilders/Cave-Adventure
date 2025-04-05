#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(0);
    ImGui::SFML::Init(window);

    auto circle = sf::CircleShape(200, 50);
    circle.setPosition(sf::Vector2f(200.0f, 200.0f));
    circle.setFillColor(sf::Color::Cyan);

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

        ImGui::SFML::Update(window, clock.restart());

        ImGui::ShowDemoWindow();

        window.clear();

        window.draw(circle);
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
}
