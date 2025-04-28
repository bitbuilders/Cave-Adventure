
#pragma once

#include "Tickable.h"

class Module : public Tickable
{
public:
    ~Module() override = default;

    /** Called when a startup module is loaded by Game, or after a module is dynamically created during runtime (right after constructor) */
    virtual void Init() {}

    /// Tickable interface start
    void Update(const sf::Time& Delta) override {}
    void LateUpdate(const sf::Time& Delta) override {}
    void FixedUpdate(float FixedDelta) override {}
    void Render(sf::RenderWindow& Window) override {}
    void Poll(const sf::Event& Event) override {}

    constexpr bool CanUpdate() override { return true; }
    constexpr bool CanLateUpdate() override { return false; }
    constexpr bool CanFixedUpdate() override { return false; }
    constexpr bool CanRender() override { return false; }
    constexpr bool CanPoll() override { return false; }
    /// Tickable interface end

    virtual constexpr std::string GetName() = 0;
};
