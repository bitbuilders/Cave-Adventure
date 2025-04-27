
#include "Game.h"

#include <imgui-SFML.h>

#include <cmath>

#include "AssetLibrary.h"
#include "DebugDraw.h"
#include "Calc.h"
#include "CaveRand.h"
#include "Controls.h"
#include "GameConfig.h"
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

    int consoleX = window.getPosition().x - 100;
    int consoleY = window.getPosition().y + static_cast<int>(window.getSize().y) - 100;
    console.Init(0, {800u, 500u}, {consoleX, consoleY});

    LoadStartupModules();

    TimedAction action;
    action.action = [](const sf::Time& delta, const sf::Time& lifetime, sf::RenderWindow* window)
    {
        LOG("Occasional log {}", delta.asSeconds());
    };
    // action.infinite = true;
    action.rate = 1.0f;
    action.duration = 2.1f;

    LoadModule<CaveChrono>("CaveChrono").TrackUpdateAction(std::move(action));

    DrawDebug::Line({100.0f, 0.0f}, {100.0f, 300.0f}, sf::Color::Green, 3.0f, 3.0f);

    TimedAction delay;
    delay.action = [](const sf::Time&, const sf::Time&, sf::RenderWindow*)
    {
        LOG("This happened next frame!");
    };
    LoadModule<CaveChrono>("CaveChrono").TrackUpdateAction(delay);

    LOG("This happened this frame!");

    PressCallback callback;
    callback.mappings = {{sf::Keyboard::Key::T, PressedInputType::Down}};
    callback.callback = [](int Player, PressedInputType::Type Input)
    {
        LOG("Callback for T held! {}", static_cast<int>(Input));
    };

    Controls::Get().ListenForPress(callback);

    AxisCallback axisCallback;
    axisCallback.mappings = {{MouseAxis::X}};
    axisCallback.callback = [](int player, float NewValue, float OldValue)
    {
        // LOG("Mouse move X {}", NewValue - OldValue);
    };

    Controls::Get().ListenForAxis(axisCallback);
    AxisCallback axisCallback2;
    axisCallback2.mappings = {{GamepadAxis::RSY}};
    axisCallback2.callback = [](int player, float NewValue, float OldValue)
    {
        // LOG("RSY callback {} <= {}", NewValue, OldValue);
    };

    Controls::Get().ListenForAxis(axisCallback2);

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

        for (auto module : modules)
        {
            if (module->CanPoll())
            {
                module->Poll(*event);
            }
        }

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

    for (auto module : modules)
    {
        if (module->CanUpdate())
        {
            module->Update(delta);
        }
    }

    auto time = GetTime();
    temp.x = std::sin(time) * 200.0f + 200.0f;
    temp.y = std::cos(time) * 100.0f + 100.0f;

    console.Update(delta);
    ResetImGuiWindow();

    ///////// Late update

    SetTickPhase(TickPhase::LateUpdate);

    for (auto module : modules)
    {
        if (module->CanLateUpdate())
        {
            module->LateUpdate(delta);
        }
    }
}

void Game::FixedUpdate()
{
    SetTickPhase(TickPhase::FixedUpdate);

    for (auto module : modules)
    {
        if (module->CanFixedUpdate())
        {
            module->FixedUpdate(GameConfig::FixedUpdateInterval);
        }
    }
}

void Game::Render()
{
    SetTickPhase(TickPhase::Render);

    for (auto module : modules)
    {
        if (module->CanRender())
        {
            module->Render(window);
        }
    }

    auto square = sf::RectangleShape({200, 100});
    square.setFillColor(sf::Color::Magenta);
    square.setPosition(temp);

    window.draw(square);

    DrawDebug::LineSegment({{100.0f, 500.0f}, {200.0f, 500.0f}, { 400.0f, 700.0f}, { 600.0f, 700.0f} }, sf::Color::White, 10.0f);

    DrawDebug::Line({900.0f, 200.0f}, {700.0f, 900.0f}, sf::Color::Yellow);

    // DrawDebug::LineSegment({{450.0f, 400.0f}, {600.0f, 400.0f}, {600.0f, 800.0f}, {450.0f, 800.0f}, {400.0f, 400.0f}}, sf::Color::Red);

    DrawDebug::Rect({400, 600}, {200, 50}, sf::Color::White, 10.0f);

    DrawDebug::Circle({900, 900}, 400, sf::Color::Yellow, 20);

    console.Render();
}

void Game::LoadStartupModules()
{
    for (auto module : STARTUP_MODULES::Modules)
    {
        modules.push_back(module);
    }

    STARTUP_MODULES::Modules.clear();
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

// namespace STARTUP_MODULES
// {
//     std::vector<Module*> Modules;
// }
