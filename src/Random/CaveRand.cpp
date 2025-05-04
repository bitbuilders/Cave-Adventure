
#include "CaveRand.h"

#include "Calc.h"
#include "Game.h"
#include "ModuleContainer.h"

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

void Mystery::Reseed(std::optional<uint64_t> Seed)
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

Mystery::Mystery(uint64_t Seed)
{
    Reseed(Seed);
}

CaveRand& CaveRand::Get()
{
    return ModuleContainer::Get().LoadModule<CaveRand>("Random");
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
    float x = Normalize ? 1.0f : Value();
    float y = 0.0f;
    float angle = Value(360.0f);

    return Math::Rotate({x, y}, angle);
}

sf::Color CaveRand::Color(uint8_t Min, bool Alpha)
{
    const uint8_t r = Range(Min, 255);
    const uint8_t g = Range(Min, 255);
    const uint8_t b = Range(Min, 255);
    const uint8_t a = Alpha ? Range(Min, 255) : 255;

    return { r, g, b, a };
}
