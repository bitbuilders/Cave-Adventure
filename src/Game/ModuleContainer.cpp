
#include "ModuleContainer.h"

#include "Controls.h"
#include "Game.h"

std::unique_ptr<ModuleContainer> ModuleContainer::instance;

ModuleContainer& ModuleContainer::Get()
{
    if (!instance)
    {
        instance = std::make_unique<ModuleContainer>();
    }

    return *instance;
}

void ModuleContainer::Init()
{
    initialized = true;

    for (auto module : modules)
    {
        LOG("Initializing startup module: {}", module->GetName());

        module->Init();
    }
}

void ModuleContainer::Update(const sf::Time& Delta)
{
    if (!asyncModules.empty())
    {
        modules.append_range(asyncModules);
        asyncModules.clear();
    }

    ++moduleLoopCount;
    for (auto module : modules)
    {
        if (module->CanUpdate())
        {
            module->Update(Delta);
        }
    }
    --moduleLoopCount;
}

void ModuleContainer::LateUpdate(const sf::Time& Delta)
{
    ++moduleLoopCount;
    for (auto module : modules)
    {
        if (module->CanLateUpdate())
        {
            module->LateUpdate(Delta);
        }
    }
    --moduleLoopCount;
}

void ModuleContainer::FixedUpdate(float FixedDelta)
{
    ++moduleLoopCount;
    for (auto module : modules)
    {
        if (module->CanFixedUpdate())
        {
            module->FixedUpdate(FixedDelta);
        }
    }
    --moduleLoopCount;
}

void ModuleContainer::Render(sf::RenderWindow& Window)
{
    ++moduleLoopCount;
    for (auto module : modules)
    {
        if (module->CanRender())
        {
            module->Render(Window);
        }
    }
    --moduleLoopCount;
}

void ModuleContainer::Poll(const sf::Event& Event)
{
    ++moduleLoopCount;
    for (auto module : modules)
    {
        if (module->CanPoll())
        {
            module->Poll(Event);
        }
    }
    --moduleLoopCount;
}
