
#include "Game.h"

#include <imgui-SFML.h>

#include <cmath>

#include "AssetLibrary.h"
#include "DebugDraw.h"

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

    TimedAction action;
    action.action = [](const sf::Time& delta, const sf::Time& lifetime, sf::RenderWindow* window)
    {
        LOG("Occasional log {}", delta.asSeconds());
    };
    // action.infinite = true;
    action.rate = 1.0f;
    action.duration = 2.1f;

    chrono.TrackUpdateAction(std::move(action));

    DrawDebug::Line({100.0f, 0.0f}, {100.0f, 300.0f}, sf::Color::Green, 3.0f, 3.0f);

    TimedAction delay;
    delay.action = [](const sf::Time&, const sf::Time&, sf::RenderWindow*)
    {
        LOG("This happened next frame!");
    };
    chrono.TrackUpdateAction(delay);

    LOG("This happened this frame!");
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
        if (event->is<sf::Event::Closed>())
        {
            Shutdown();
        }
    }

    if (!IsRunning())
    {
        return;
    }

    sf::Time delta = frameClock.restart();

    Update(delta);

    ImGui::SFML::SetCurrentWindow(window);
    ImGui::SFML::Update(window, delta);

    ImGui::ShowDemoWindow();

    window.clear();

    Render();
    ImGui::SFML::Render(window);

    window.display();
}

void Game::Update(const sf::Time& delta)
{
    chrono.Update(delta);

    temp.x = std::sin(clock.getElapsedTime().asSeconds()) * 200.0f + 200.0f;
    temp.y = std::cos(clock.getElapsedTime().asSeconds()) * 100.0f + 100.0f;

    console.Update(delta);
}

void Game::Render()
{
    chrono.Render(window);

    auto square = sf::RectangleShape({200, 100});
    square.setFillColor(sf::Color::Magenta);
    square.setPosition(temp);

    window.draw(square);

    DrawDebug::LineSegment({{100.0f, 500.0f}, {200.0f, 500.0f}, { 400.0f, 700.0f}, { 600.0f, 700.0f} }, sf::Color::White, 10.0f);

    DrawDebug::Line({900.0f, 200.0f}, {700.0f, 900.0f}, sf::Color::Yellow);

    DrawDebug::LineSegment({{450.0f, 400.0f}, {600.0f, 400.0f}, {600.0f, 800.0f}, {450.0f, 800.0f}, {400.0f, 400.0f}}, sf::Color::Red);

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

CaveChrono& Game::GetChrono()
{
    return chrono;
}
