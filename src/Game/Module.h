
#pragma once

#include "Console.h"
#include "Tickable.h"

#include <memory>

/**
 * Create a startup module, should be in global space
 * (loads on app start, rather than manually from Game nor dynamically at runtime)
 */
#define MAKE_STARTUP(Module) inline DummyStartupModule<Module> __##Module##Dummy;

class Module : public Tickable
{
public:
    ~Module() override = default;

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


namespace STARTUP_MODULES
{
    inline std::vector<std::shared_ptr<Module>> Modules;
}

/* This is to allow modules to be created globally, without Game needing to do it */
template<class T>
class DummyStartupModule
{
public:
    DummyStartupModule()
    {
        STARTUP_MODULES::Modules.emplace_back(std::make_shared<T>());
    }
};

