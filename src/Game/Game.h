
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "CaveChrono.h"
#include "Console.h"
#include "Effect.h"


enum class TickPhase
{
    PreUpdate,
    Update,
    LateUpdate,
    FixedUpdate,
    PreRender,
    Render
};


class Game
{
public:
    static Game& Get();

private:
    static Game* game;

private:
    TickPhase phase = TickPhase::PreUpdate;

    sf::Clock clock;

    sf::Clock frameClock;

    sf::Time currentDelta;

    Console console;

    sf::Music music;

    sf::Vector2f temp;

    sf::RenderWindow window;

    Effect effect;

public:
    void Init();

    void Shutdown();

    void Tick();

    void Update(const sf::Time& delta);

    void FixedUpdate();

    void Render();

    bool IsRunning() const;

    sf::RenderWindow& GetWindow();

    /** Sets the ImGui::SFML window to the main game window */
    void ResetImGuiWindow();

    /** Get the time since application start, in seconds (unaffected by time dilation) */
    float GetTime() const;

    /** Get this frame's delta time */
    float GetDeltaTime() const;

    TickPhase GetTickPhase() const;

    bool InRenderPhase() const;

private:
    void SetTickPhase(TickPhase Phase);
};
