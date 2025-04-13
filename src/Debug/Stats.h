
#pragma once

#include <string>

#include <SFML/System/Clock.hpp>

struct ScopedTimer
{
private:
    std::string tag;

    sf::Clock clock;

public:
    ScopedTimer(std::string&& Tag);
    ~ScopedTimer();

    void Start();
};
