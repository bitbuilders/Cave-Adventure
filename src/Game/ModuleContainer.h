
#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <functional>

#include "Module.h"

#define CREATE_STARTUP_MODULE(Class) StartupModule<Class> __##Class##StartupModule;

class ModuleContainer : public Tickable
{
public:
    static ModuleContainer& Get();

private:
    static std::unique_ptr<ModuleContainer> instance;

private:
    std::vector<std::shared_ptr<Module>> modules;

    /** Modules that are added during loop iteration */
    std::vector<std::shared_ptr<Module>> asyncModules;

    int moduleLoopCount = 0;

public:
    template<class ModuleClass>
    void AddModule()
    {
        static_assert(std::is_base_of_v<Module, ModuleClass>, "Template type is not a Module!");

        if (moduleLoopCount > 0)
        {
            std::shared_ptr<ModuleClass> module = std::make_shared<ModuleClass>();
            module->Init();
            asyncModules.push_back(std::move(module));
        }
        else
        {
            std::shared_ptr<ModuleClass> module = std::make_shared<ModuleClass>();
            module->Init();
            modules.push_back(std::move(module));
        }
    }

    void RemoveModule(const std::string& Name)
    {
        auto removeModules = std::ranges::remove(modules, Name, &Module::GetName);
        modules.erase(removeModules.begin(), removeModules.end());
        auto removeAsyncModules = std::ranges::remove(asyncModules, Name, &Module::GetName);
        modules.erase(removeAsyncModules.begin(), removeAsyncModules.end());
    }

    Module* GetModule(const std::string& Name)
    {
        auto foundModule = std::ranges::find(modules, Name, &Module::GetName);
        if (foundModule == modules.end())
        {
            auto foundAsyncModule = std::ranges::find(asyncModules, Name, &Module::GetName);
            return foundAsyncModule != asyncModules.end() ? foundAsyncModule->get() : nullptr;
        }

        return foundModule->get();
    }

    template<class ModuleClass>
    ModuleClass* GetModule(const std::string& Name)
    {
        static_assert(std::is_base_of_v<Module, ModuleClass>, "Template type is not a Module!");

        return static_cast<ModuleClass*>(GetModule(Name));
    }

    template<class ModuleClass>
    ModuleClass& LoadModule(const std::string& Name)
    {
        static_assert(std::is_base_of_v<Module, ModuleClass>, "Template type is not a Module!");

        ModuleClass* foundModule = GetModule<ModuleClass>(Name);
        if (!foundModule)
        {
            AddModule<ModuleClass>();
            foundModule = GetModule<ModuleClass>(Name);
            CHECK(foundModule, "Module does not exist with name");
        }

        return *foundModule;
    }

    /// Tickable interface start
    void Update(const sf::Time& Delta) override;
    void LateUpdate(const sf::Time& Delta) override;
    void FixedUpdate(float FixedDelta) override;
    void Render(sf::RenderWindow& Window) override;
    void Poll(const sf::Event& Event) override;

    constexpr bool CanUpdate() override { return true;}
    constexpr bool CanLateUpdate() override { return true; }
    constexpr bool CanFixedUpdate() override { return true; }
    constexpr bool CanRender() override { return true; }
    constexpr bool CanPoll() override { return true; }
    /// Tickable interface end
};

/** A simple wrapper for creating "startup modules". To be used with the CREATE_STARTUP_MODULE macro, so modules can be created at initialization */
template<class ModuleType>
class StartupModule
{
public:
    StartupModule()
    {
        ModuleContainer::Get().AddModule<ModuleType>();
    }
};
