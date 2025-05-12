
#pragma once

#include "Module.h"
#include "LivePic.h"
#include "SFML/Graphics/Texture.hpp"

class Cave : public Module
{
public:
    constexpr std::string GetName() override { return "Cave"; }

private:
    sf::Texture texture;
    LivePic player;

public:
    void Init() override;

    void Update(const sf::Time& Delta) override;
    void Render(sf::RenderWindow& Window) override;

    constexpr bool CanRender() override { return true; }

    Cave();
};
