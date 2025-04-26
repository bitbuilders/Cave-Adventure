
#pragma once

#include "Module.h"

#include <random>
#include <optional>

#include "SFML/System/Vector2.hpp"


struct Mystery
{
private:
    std::mt19937_64 generator;

public:
    /* Return a float between 0.0 (inclusive) and Max (exclusive) */
    float Value(float Max = 1.0f);

    /* Return a float between Min (inclusive) and Max (exclusive) */
    float Range(float Min, float Max);

    /* Return an int between Min (inclusive) and Max (inclusive) */
    int Range(int Min, int Max);

    /* Advances generator by Num generations */
    void SkipNext(uint64_t Num = 1);

    /* Resets the seed on the generator with an optionally provided seed */
    void Reseed(std::optional<int64_t> Seed = {});

    Mystery();
    explicit Mystery(int64_t Seed);
};

class CaveRand : public Module
{
private:
    Mystery mystery;

public:
    static CaveRand& Get();

    constexpr std::string GetName() override { return "Random"; }

public:
    /* Return a float between 0.0 (inclusive) and Max (exclusive) */
    float Value(float Max = 1.0f);

    /* Return a float between Min (inclusive) and Max (exclusive) */
    float Range(float Min, float Max);

    /* Return an int between Min (inclusive) and Max (inclusive) */
    int Range(int Min, int Max);

    /* Return a random vector within a unit, or optionally normalized */
    sf::Vector2f InsideCircle(bool Normalize = false);
};
