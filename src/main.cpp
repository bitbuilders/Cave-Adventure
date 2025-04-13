#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "AssetLibrary.h"
#include "Game/Game.h"

#ifdef WIN32

// See bottom of file
#include <windows.h>

#endif

int main(int argc, char* argv[])
{
    auto window = sf::RenderWindow(
        sf::VideoMode({1920u, 1080u}),
        "Adventure",
        sf::Style::Default,
        sf::State::Windowed);
    window.setFramerateLimit(0);
    if (!ImGui::SFML::Init(window, false))
    {
        return 1;
    }

    // AssetLibrary::LoadFonts();

    if (!ImGui::SFML::UpdateFontTexture())
    {
        // NO FONT!
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

        //// Update

        sf::Time delta = clock.restart();
        game.Update(window, delta);

        ImGui::SFML::SetCurrentWindow(window);
        ImGui::SFML::Update(window, delta);

        ImGui::ShowDemoWindow();

        //// Draw

        window.clear();

        game.Render(window);
        ImGui::SFML::Render(window);

        window.display();
    }

    game.Shutdown();
    ImGui::SFML::Shutdown();

    return 0;
}


#ifdef WIN32

// Real entry point for Windows app, prevents console from loading (hopefully cross-platform compatible because of preprocessors?)
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    return main(__argc, __argv);
}

#endif
