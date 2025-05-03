
#pragma once

#include "Tickable.h"

class Module : public Tickable
{
public:
    ~Module() override = default;

    /** Called when a module is initialized, which can be at Game initialization (startup module) or right after module creation at runtime */
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
