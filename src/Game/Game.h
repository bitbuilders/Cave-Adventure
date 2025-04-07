
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class Game {
public:
    void Init(sf::RenderWindow& window);

    void Update(sf::RenderWindow& window, const sf::Time& delta);

    void Render(sf::RenderWindow& window);

private:
    sf::Clock clock;

    sf::Music music;

    sf::Vector2f temp;
};

