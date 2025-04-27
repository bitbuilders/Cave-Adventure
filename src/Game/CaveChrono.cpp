
#include "CaveChrono.h"

#include <algorithm>

#include "Game.h"

namespace Chrono
{
    constexpr uint64_t MaxHandles = 1000000;
}

TimedActionHandle TimedAction::currentHandle = 0;

TimedAction::TimedAction() :
    id(0)
{
    lifetime.start();
    interval.start();
}

sf::RenderWindow* TimedAction::GetWindow(sf::RenderWindow* Default) const
{
    return window ? window : Default;
}

CaveChrono& CaveChrono::Get()
{
    return Game::Get().LoadModule<CaveChrono>("CaveChrono");
}

TimedActionHandle CaveChrono::TrackUpdateAction(TimedAction&& Action, bool CallImmediately)
{
    auto& action = updateActions.emplace_back(std::move(Action));
    action.id = GetNextHandle();

    if (CallImmediately && action.action)
    {
        action.action(action.lifetime.getElapsedTime(),
            action.lifetime.getElapsedTime(),
            action.GetWindow(&Game::Get().GetWindow()));
    }

    return action.id;
}

TimedActionHandle CaveChrono::TrackUpdateAction(TimedAction& Action, bool CallImmediately)
{
    return TrackUpdateAction(std::move(Action), CallImmediately);
}

TimedActionHandle CaveChrono::TrackRenderAction(TimedAction&& Action, bool CallImmediately)
{
    auto& action = renderActions.emplace_back(std::move(Action));
    action.id = GetNextHandle();

    if (CallImmediately && Game::Get().InRenderPhase() && action.action)
    {
        action.action(action.lifetime.getElapsedTime(),
            action.lifetime.getElapsedTime(),
            action.GetWindow(&Game::Get().GetWindow()));
    }

    return action.id;
}

TimedActionHandle CaveChrono::TrackRenderAction(TimedAction& Action, bool CallImmediately)
{
    return TrackRenderAction(std::move(Action), CallImmediately);
}

bool CaveChrono::CancelUpdateAction(TimedActionHandle Handle)
{
    size_t sizeBefore = updateActions.size();
    auto removeActions = std::ranges::remove_if(updateActions, [Handle](const TimedAction& Action)
    {
       return Action.id == Handle;
    });
    updateActions.erase(removeActions.begin(), removeActions.end());

    return sizeBefore > updateActions.size();
}

bool CaveChrono::CancelRenderAction(TimedActionHandle Handle)
{
    size_t sizeBefore = renderActions.size();
    auto removeActions = std::ranges::remove_if(renderActions, [Handle](const TimedAction& Action)
    {
       return Action.id == Handle;
    });
    renderActions.erase(removeActions.begin(), removeActions.end());

    return sizeBefore > renderActions.size();
}

void CaveChrono::Update(const sf::Time& Delta)
{
    for (auto& action : updateActions)
    {
        auto interval = action.interval.getElapsedTime().asSeconds();
        if ((action.rate <= 0.0f || interval >= action.rate - action.rateTolerance) ||
            (!action.infinite && action.duration <= 0.0f))
        {
            sf::Time rate = action.interval.restart();
            if (action.action)
            {
                action.action(rate, action.lifetime.getElapsedTime(), action.GetWindow(&Game::Get().GetWindow()));
            }
        }
    }

    auto expiredActions = std::ranges::remove_if(updateActions,
       [](const TimedAction& Action)
       {
           auto lifetime = Action.lifetime.getElapsedTime().asSeconds();
           return !Action.infinite && lifetime >= Action.duration;
       });
    updateActions.erase(expiredActions.begin(), expiredActions.end());
}

void CaveChrono::Render(sf::RenderWindow& Window)
{
    for (auto& action : renderActions)
    {
        auto interval = action.interval.getElapsedTime().asSeconds();
        if ((action.rate <= 0.0f || interval >= action.rate - action.rateTolerance) ||
            (!action.infinite && action.duration <= 0.0f))
        {
            sf::Time rate = action.interval.restart();
            if (action.action)
            {
                action.action(rate, action.lifetime.getElapsedTime(), action.GetWindow(&Window));
            }
        }
    }

    auto expiredActions = std::ranges::remove_if(renderActions,
        [](const TimedAction& Action)
        {
           auto lifetime = Action.lifetime.getElapsedTime().asSeconds();
           return !Action.infinite && lifetime >= Action.duration;
        });
    renderActions.erase(expiredActions.begin(), expiredActions.end());
}

TimedActionHandle CaveChrono::GetNextHandle()
{
    ++TimedAction::currentHandle;

    if (TimedAction::currentHandle >= Chrono::MaxHandles)
    {
        TimedAction::currentHandle = 0;
    }

    auto handleExists = [](const TimedAction& Action)
    {
        return Action.id == TimedAction::currentHandle;
    };

    while (std::ranges::any_of(updateActions, handleExists) ||
        std::ranges::any_of(renderActions, handleExists))
    {
        TimedAction::currentHandle++;
    }

    return TimedAction::currentHandle;
}
