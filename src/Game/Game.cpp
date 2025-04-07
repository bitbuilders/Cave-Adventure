
#include "Game.h"

#include <iostream>

void Game::Init(sf::RenderWindow& window)
{
    std::cout << "GAME INIT\n";
    clock.restart();

    // todo: figure out how to embed music (and other stuff) into .exe
    if (music.openFromFile("music.mp3"))
    {
        music.setVolume(20.0f);
        // music.play();
    }
}

void Game::Update(sf::RenderWindow& window, const sf::Time& delta)
{
    temp.x = sin(clock.getElapsedTime().asSeconds()) * 200.0f + 200.0f;
    temp.y = cos(clock.getElapsedTime().asSeconds()) * 100.0f + 100.0f;
}

void Game::Render(sf::RenderWindow& window)
{
    auto square = sf::RectangleShape({200, 100});
    square.setFillColor(sf::Color::Magenta);
    square.setPosition(temp);

    window.draw(square);
}
