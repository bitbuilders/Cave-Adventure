
#include "Game.h"

#include <cmath>

void Game::Init(sf::RenderWindow& window)
{
    // std::cout << "GAME INIT\n";
    clock.restart();

    // todo: figure out how to embed music (and other stuff) into .exe
    if (music.openFromFile("assets/audio/music.mp3"))
    {
        music.setVolume(20.0f);
        // music.play();
    }

    int consoleX = window.getPosition().x - 100;
    int consoleY = window.getPosition().y + static_cast<int>(window.getSize().y) - 100;
    console.Init(-1, {600u, 400u}, {consoleX, consoleY});
}

void Game::Shutdown()
{
    // std::cout << "GAME SHUTDOWN\n";
    console.Shutdown();
}

void Game::Update(sf::RenderWindow& window, const sf::Time& delta)
{
    temp.x = std::sin(clock.getElapsedTime().asSeconds()) * 200.0f + 200.0f;
    temp.y = std::cos(clock.getElapsedTime().asSeconds()) * 100.0f + 100.0f;

    console.Update(delta);
}

void Game::Render(sf::RenderWindow& window)
{
    auto square = sf::RectangleShape({200, 100});
    square.setFillColor(sf::Color::Magenta);
    square.setPosition(temp);

    window.draw(square);

    console.Render();
}
