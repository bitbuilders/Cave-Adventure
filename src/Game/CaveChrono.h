
#pragma once

#include <functional>
#include <vector>

#include "Module.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"


using TimedActionHandle = uint64_t;

struct TimedAction
{
    std::function<void(
        const sf::Time& /*Delta*/,
        const sf::Time& /*ActionLifetime*/,
        sf::RenderWindow* /*DrawWindow*/)> action;

    /** How often this action will be invoked, in seconds (<= 0 is every frame) */
    float rate = 0.0f;

    /** <= 0 will call next tick only */
    float duration = 0.0f;

    /** The action will be invoked if the time is within this tolerance */
    float rateTolerance = 0.00001f;

    /** Will go until it is manually stopped (overrides duration) */
    bool infinite = false;

    /** An optional window to have passed in as a parameter (otherwise the default window will be used) */
    sf::RenderWindow* window = nullptr;

    TimedAction();

private:
    sf::Clock lifetime;

    sf::Clock interval;

    TimedActionHandle id;

    sf::RenderWindow* GetWindow(sf::RenderWindow* Default) const;

private:
    static TimedActionHandle currentHandle;

    friend class CaveChrono;
};

class CaveChrono final : public Module
{
public:
    static CaveChrono& Get();

    /// Module interface start
    constexpr std::string GetName() override { return "CaveChrono"; }
    /// Module interface end

private:
    std::vector<TimedAction> updateActions;
    std::vector<TimedAction> renderActions;

public:
    /** Queue a timed action to be invoked for a specified period of time */
    TimedActionHandle TrackUpdateAction(TimedAction&& Action, bool CallImmediately = false);

    /** Don't use the passed in action after calling this function! */
    TimedActionHandle TrackUpdateAction(TimedAction& Action, bool CallImmediately = false);

    /**
     * Queue a timed action to be invoked during the render phase of the game loop
     *
     * If called from update-cycle the action will be invoked in the same frame (after updates)
     */
    TimedActionHandle TrackRenderAction(TimedAction&& Action, bool CallImmediately = false);

    /** Don't use the passed in action after calling this function! */
    TimedActionHandle TrackRenderAction(TimedAction& Action, bool CallImmediately = false);

    bool CancelUpdateAction(TimedActionHandle Handle);

    bool CancelRenderAction(TimedActionHandle Handle);

    /// Tickable interface start
    void Update(const sf::Time& Delta) override;

    void Render(sf::RenderWindow& Window) override;

    constexpr bool CanUpdate() override { return true; }
    constexpr bool CanRender() override { return true; }
    /// Tickable interface end

private:
    TimedActionHandle GetNextHandle();
};
