
#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

struct Animation
{
    /* The row the animation is in */
    int row = 0;
    /* The starting column */
    int offset = 0;
    /* How long the animation is */
    int frames = 1;
    /* How often each frame lasts in seconds */
    float frameTime = 1.0f / 20.0f;

private:
    int currentFrame = 0;
    float time = 0.0f;

    friend class LivePic;
};

class LivePic : public sf::Sprite
{
    Animation animation;

    sf::Vector2i size;

public:
    void SetAnimation(const Animation& Animation);

    void SetSize(const sf::Vector2i& Size);

    void Update(const sf::Time& Delta);

    LivePic(const sf::Texture& Texture);

private:
    void Refresh();
};
