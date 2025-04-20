
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <type_traits>
#include <memory>

#include "CaveChrono.h"
#include "Console.h"
#include "Module.h"


class Game
{
public:
    static Game& Get();

private:
    static Game* game;

private:
    sf::Clock clock;

    sf::Clock frameClock;

    Console console;

    sf::Music music;

    sf::Vector2f temp;

    sf::RenderWindow window;

    std::vector<std::shared_ptr<Module>> modules;

public:
    void Init();

    void Shutdown();

    void Tick();

    void Update(const sf::Time& delta);

    void Render();

    bool IsRunning() const;

    sf::RenderWindow& GetWindow();

    template<class ModuleClass>
    void AddModule()
    {
        static_assert(std::is_base_of_v<Module, ModuleClass>, "Template type is not a Module!");

        modules.emplace_back(std::make_shared<ModuleClass>());
    }

    void RemoveModule(const std::string& Name)
    {
        auto removeModules = std::ranges::remove(modules, Name, &Module::GetName);
        modules.erase(removeModules.begin(), removeModules.end());
    }

    Module* GetModule(const std::string& Name)
    {
        auto foundModule = std::ranges::find(modules, Name, &Module::GetName);
        return foundModule != modules.end() ? foundModule->get() : nullptr;
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
};
