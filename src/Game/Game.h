
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "CaveChrono.h"
#include "Console.h"


class Game
{
public:
    static Game& Get();

private:
    static Game* game;

private:
    sf::Clock clock;

    sf::Clock frameClock;

    Console console;

    CaveChrono chrono;

    sf::Music music;

    sf::Vector2f temp;

    sf::RenderWindow window;

public:
    void Init();

    void Shutdown();

    void Tick();

    void Update(const sf::Time& delta);

    void Render();

    bool IsRunning() const;

    sf::RenderWindow& GetWindow();

    CaveChrono& GetChrono();
};
