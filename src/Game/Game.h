
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Console.h"


class Game
{
public:
    void Init(sf::RenderWindow& window);

    void Shutdown();

    void Update(sf::RenderWindow& window, const sf::Time& delta);

    void Render(sf::RenderWindow& window);

private:
    sf::Clock clock;

    Console console;

    sf::Music music;

    sf::Vector2f temp;
};

