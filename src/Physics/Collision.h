
#pragma once

#include "Module.h"

#include "Game.h"

class Collision final : public Module
{
public:
    constexpr std::string GetName() override { return "Collision"; }

public:
    void FixedUpdate(float FixedDelta) override;

    bool CanUpdate() override { return false; }
    bool CanFixedUpdate() override { return true; }

};
