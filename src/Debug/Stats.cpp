
#include "Stats.h"

#include <format>

#include "Console.h"


ScopedTimer::ScopedTimer(std::string&& Tag) :
    tag(std::move(Tag))
{
    Start();
}

ScopedTimer::~ScopedTimer()
{
    std::string label = tag.empty() ? "" : std::format("{} ", tag);

    auto delta = clock.getElapsedTime();

    if (delta.asSeconds() >= 1.0f)
    {
        LOG_STAT("{}Took {}s", label, delta.asSeconds());
    }
    else if (delta.asMilliseconds() >= 1)
    {
        LOG_STAT("{}Took {}ms", label, delta.asMilliseconds());
    }
    else
    {
        LOG_STAT("{}Took {} micro seconds", label, delta.asMicroseconds());
    }
}

void ScopedTimer::Start()
{
    clock.restart();
}
