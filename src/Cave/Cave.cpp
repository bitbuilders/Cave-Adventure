
#include "Cave.h"

#include "Calc.h"
#include "Console.h"
#include "Controls.h"
#include "ModuleContainer.h"

MAKE_STARTUP_MODULE(Cave)

void Cave::Init()
{
    if (texture.loadFromFile("assets/third party/Tiny RPG Character Asset Pack v1.03/Soldier/Soldier/Soldier.png"))
    {
        Animation idle;
        idle.row = 1;
        idle.frames = 8;
        idle.frameTime = 1.0f / 10.0f;
        player.SetAnimation(idle);
        player.SetSize({100, 100});

        player.setPosition({400, 400});
        player.setScale({5, 5});
    }
}

void Cave::Update(const sf::Time& Delta)
{
    player.Update(Delta);

    sf::Vector2f velocity;
    if (Controls::Get().GetPressedState(sf::Keyboard::Key::A, PressedInputType::Down))
    {
        velocity.x += -1.0f;
    }
    if (Controls::Get().GetPressedState(sf::Keyboard::Key::D, PressedInputType::Down))
    {
        velocity.x += 1.0f;
    }
    if (Controls::Get().GetPressedState(sf::Keyboard::Key::W, PressedInputType::Down))
    {
        velocity.y += -1.0f;
    }
    if (Controls::Get().GetPressedState(sf::Keyboard::Key::S, PressedInputType::Down))
    {
        velocity.y += 1.0f;
    }

    if (!Math::NearlyZero(velocity))
    {
        player.move(velocity.normalized() * 300.0f * Delta.asSeconds());
    }
}

void Cave::Render(sf::RenderWindow& Window)
{
    Window.draw(player);
}

Cave::Cave() :
    player(texture)
{
}
