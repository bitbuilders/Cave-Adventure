
#include "Game.h"

#include <imgui-SFML.h>

#include <cmath>

#include "AssetLibrary.h"
#include "DebugDraw.h"
#include "Calc.h"
#include "CaveRand.h"
#include "Controls.h"
#include "GameConfig.h"
#include "ModuleContainer.h"
#include "Stats.h"

Game* Game::game;

Game& Game::Get()
{
    if (game)
    {
        return *game;
    }

    LOG_FATAL("Game does not exist!");

    static Game invalidGame;
    return invalidGame;
}

void Game::Init()
{
    game = this;

    window = sf::RenderWindow(
        sf::VideoMode({1920u, 1080u}),
        "Adventure",
        sf::Style::Default,
        sf::State::Windowed);
    window.setFramerateLimit(0);
    if (!ImGui::SFML::Init(window, false))
    {
        return;
    }

    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    AssetLibrary::LoadFonts();

    clock.restart();
    frameClock.restart();

    // todo: figure out how to embed music (and other stuff) into .exe
    if (music.openFromFile("assets/audio/music.mp3"))
    {
        music.setVolume(20.0f);
        // music.play();
    }

    Console::EmptyLogFile();

    LOG("Game Init");

    ModuleContainer::Get().Init();

    int consoleX = window.getPosition().x - 800;
    int consoleY = window.getPosition().y + static_cast<int>(window.getSize().y) - 100;
    console.Init(0, {800u, 500u}, {consoleX, consoleY});

    TimedAction fixedUpdate;
    fixedUpdate.infinite = true;
    fixedUpdate.rate = GameConfig::FixedUpdateInterval;
    fixedUpdate.action = [this](const sf::Time&, const sf::Time&, sf::RenderWindow*)
    {
        FixedUpdate();

        SetTickPhase(TickPhase::Update); // Reset tick phase
    };
    CaveChrono::Get().TrackUpdateAction(std::move(fixedUpdate));
}

void Game::Shutdown()
{
    LOG("Game Shutdown");

    ImGui::SFML::Shutdown(window);

    window.close();

    console.Shutdown();

    Console::CleanupLog();
}

void Game::Tick()
{
    while (const auto event = window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(window, *event);

        ModuleContainer::Get().Poll(*event);

        if (event->is<sf::Event::Closed>())
        {
            Shutdown();
        }
    }

    if (!IsRunning())
    {
        return;
    }

    currentDelta = frameClock.restart();

    SetTickPhase(TickPhase::PreUpdate);

    ImGui::SFML::SetCurrentWindow(window);
    ImGui::SFML::Update(window, currentDelta);

    ImGui::ShowDemoWindow();

    Update(currentDelta);

    window.clear();

    SetTickPhase(TickPhase::PreRender);

    Render();
    ImGui::SFML::Render(window);

    window.display();
}

void Game::Update(const sf::Time& delta)
{
    SetTickPhase(TickPhase::Update);

    ModuleContainer::Get().Update(delta);

    auto time = GetTime();
    temp.x = std::sin(time) * 200.0f + 200.0f;
    temp.y = std::cos(time) * 100.0f + 100.0f;

    console.Update(delta);
    ResetImGuiWindow();

    ///////// Late update

    SetTickPhase(TickPhase::LateUpdate);

    ModuleContainer::Get().LateUpdate(delta);
}

void Game::FixedUpdate()
{
    SetTickPhase(TickPhase::FixedUpdate);

    ModuleContainer::Get().FixedUpdate(GameConfig::FixedUpdateInterval);
}

void Game::Render()
{
    SetTickPhase(TickPhase::Render);

    ModuleContainer::Get().Render(window);

    console.Render();
}

bool Game::IsRunning() const
{
    return window.isOpen();
}

sf::RenderWindow& Game::GetWindow()
{
    return window;
}

void Game::ResetImGuiWindow()
{
    ImGui::SFML::SetCurrentWindow(window);
}

float Game::GetTime() const
{
    return clock.getElapsedTime().asSeconds();
}

float Game::GetDeltaTime() const
{
    return currentDelta.asSeconds();
}

TickPhase Game::GetTickPhase() const
{
    return phase;
}

bool Game::InRenderPhase() const
{
    return phase == TickPhase::Render || phase == TickPhase::PreRender;
}

void Game::SetTickPhase(TickPhase Phase)
{
    phase = Phase;
}
