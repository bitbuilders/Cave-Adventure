
#pragma once

#include <SFML/Graphics.hpp>

class Console
{
private:
    sf::RenderWindow window;

    int id = -1;

public:
    bool Init(int consoleId = -1, const sf::Vector2u& size = {600u, 400u}, const sf::Vector2i& position = {0, 0});

    void Update(const sf::Time& delta);

    void Render();

    void Shutdown();

    bool IsOpen() const;

    int GetId() const;
};
