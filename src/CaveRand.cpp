
#include "CaveRand.h"

#include "Calc.h"
#include "Game.h"

float Mystery::Value(float Max)
{
    std::uniform_real_distribution<float> distribution(0.0f, Max);

    return distribution(generator);
}

float Mystery::Range(float Min, float Max)
{
    std::uniform_real_distribution<float> distribution(Min, Max);

    return distribution(generator);
}

int Mystery::Range(int Min, int Max)
{
    std::uniform_int_distribution distribution(Min, Max);

    return distribution(generator);
}

void Mystery::SkipNext(uint64_t Num)
{
    generator.discard(Num);
}

void Mystery::Reseed(std::optional<int64_t> Seed)
{
    if (Seed.has_value())
    {
        generator.seed(Seed.value());
    }
    else
    {
        std::random_device device;
        generator.seed(device());
    }
}

Mystery::Mystery()
{
    Reseed();
}

Mystery::Mystery(int64_t Seed)
{
    Reseed(Seed);
}

CaveRand& CaveRand::Get()
{
    return Game::Get().LoadModule<CaveRand>("Random");
}

float CaveRand::Value(float Max)
{
    return mystery.Value(Max);
}

float CaveRand::Range(float Min, float Max)
{
    return mystery.Range(Min, Max);
}

int CaveRand::Range(int Min, int Max)
{
    return mystery.Range(Min, Max);
}

sf::Vector2f CaveRand::InsideCircle(bool Normalize)
{
    float x = Value() * 2.0f - 1.0f;
    float y = Value() * 2.0f - 1.0f;

    if (Normalize)
    {
        return sf::Vector2f(x, y).normalized();
    }

    return Math::ClampMagnitude({ x, y }, 1.0f);
}
