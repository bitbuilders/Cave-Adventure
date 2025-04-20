
#pragma once

#include "Console.h"
#include "Tickable.h"

class Module : public Tickable
{
public:
    ~Module() override = default;

    /// Tickable interface start
    void Update(const sf::Time& Delta) override {}
    void LateUpdate(const sf::Time& Delta) override {}
    void Render(sf::RenderWindow& Window) override {}
    void Poll(const sf::Event& Event) override {}

    constexpr bool CanUpdate() override { return true; }
    constexpr bool CanLateUpdate() override { return false; }
    constexpr bool CanRender() override { return false; }
    constexpr bool CanPoll() override { return false; }
    /// Tickable interface end

    virtual constexpr std::string GetName() = 0;
};
