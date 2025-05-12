
#include "LivePic.h"


void LivePic::SetAnimation(const Animation& Animation)
{
    animation = Animation;
    animation.time = 0.0f;
    animation.currentFrame = 0;

    Refresh();
}

void LivePic::SetSize(const sf::Vector2i& Size)
{
    size = Size;

    setOrigin({size.x * 0.5f, size.y * 0.5f});

    Refresh();
}

void LivePic::Update(const sf::Time& Delta)
{
    animation.time += Delta.asSeconds();

    if (animation.time >= animation.frameTime)
    {
        animation.time -= animation.frameTime;

        ++animation.currentFrame;
        if (animation.currentFrame >= animation.frames)
        {
            animation.currentFrame = 0;
        }

        Refresh();
    }
}

LivePic::LivePic(const sf::Texture& Texture) :
    sf::Sprite(Texture)
{
}

void LivePic::Refresh()
{
    int frameX = (animation.offset + animation.currentFrame) * size.x;
    int frameY = animation.row * size.y;
    setTextureRect(sf::IntRect({ frameX, frameY }, size));
}
