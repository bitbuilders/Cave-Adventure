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
    Game game;
    game.Init();

    while (game.IsRunning())
    {
        game.Tick();
    }

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
