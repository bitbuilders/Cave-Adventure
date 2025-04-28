
#pragma once

#include "Module.h"

#include "Game.h"
#include "Thing.h"

class Collision final : public Module
{
public:
    constexpr std::string GetName() override { return "Collision"; }

private:
    std::vector<std::shared_ptr<Thing>> things;

public:
    void Init() override;

    void FixedUpdate(float FixedDelta) override;

    bool CanUpdate() override { return false; }
    bool CanFixedUpdate() override { return true; }

private:
    void RenderThings(sf::RenderWindow* Window);


};
