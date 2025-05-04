
#include "Effect.h"

#include "CaveChrono.h"
#include "CaveRand.h"
#include "Console.h"

void Effect::Update(const sf::Time& Delta)
{
    float delta = Delta.asSeconds();
    time += delta;

    constexpr auto rate = 0.001f;
    if (time >= rate)
    {
        time -= rate;
        auto& point = points.emplace_back();
        point.position = getPosition();
        point.duration = CaveRand::Get().Range(1.0f, 3.0f);
        point.speed = CaveRand::Get().Range(100.0f, 200.0f);
        point.dir = CaveRand::Get().InsideCircle(true);
        point.color = CaveRand::Get().Color(0);

        particles.resize(points.size() * 3);
    }

    int particleIndex = 0;
    for (auto & point : points)
    {
        point.time += delta;
        point.position += point.dir * point.speed * delta;

        auto& p1 = particles[particleIndex];
        auto& p2 = particles[particleIndex + 1];
        auto& p3 = particles[particleIndex + 2];
        particleIndex += 3;

        p1.position = point.position + sf::Vector2f(0.0f, -point.size * 0.5f);
        p2.position = point.position + sf::Vector2f(-point.size * 0.5f, point.size * 0.5f);
        p3.position = point.position + sf::Vector2f(point.size * 0.5f, point.size * 0.5f);

        point.color.a = static_cast<uint8_t>((1.0f - (point.time / point.duration)) * 255);
        p1.color = point.color;
        p2.color = point.color;
        p3.color = point.color;
    }

    auto remove = std::ranges::remove_if(points, [](const TimedPoint& point)
    {
        return point.time >= point.duration;
    });
    int oldSize = points.size();
    points.erase(remove.begin(), remove.end());
    if (oldSize > points.size())
    {
        particles.resize(points.size() * 3);
    }
}

void Effect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // states.transform *= getTransform();
    states.texture = nullptr;

    target.draw(particles, states);
}

Effect::Effect() :
    particles(sf::PrimitiveType::Triangles, 1000),
    time(0.0f)
{
}
