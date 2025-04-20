
#pragma once

#include "Console.h"
#include "Tickable.h"

class Module : public Tickable
{
public:
    ~Module() override = default;

    void Update(const sf::Time& Delta) override {}
    void Render(sf::RenderWindow& Window) override {}

    constexpr bool CanUpdate() override { return true; }
    constexpr bool CanRender() override { return false; }

    virtual constexpr std::string GetName() = 0;
};
