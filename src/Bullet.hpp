#pragma once

#include <SFML/Graphics.hpp>

class Bullet
{
public:
    Bullet(const sf::Vector2f &position)
    {
        bulletPicture.setPosition(position);
    }

    void update()
    {
        if (isActive)
        {
            // bulletPicture.setPosition(parabolicPath[currentStep]);
            // currentStep++;
            // if (currentStep == parabolicPath.size())
            // {
            //     isActive = false;
            // }
        }
    }

    void draw(sf::RenderTarget &target) const
    {
        if (isActive)
        {
            target.draw(bulletPicture);
        }
    }

    void setActive(bool active)
    {
        isActive = active;
    }

    void setPosition(const sf::Vector2f &position)
    {
        bulletPicture.setPosition(position);
    }

private:
    sf::CircleShape bulletPicture{4.f};
    std::vector<sf::Vector2f> parabolicPath;
    uint32_t currentStep = 0;
    bool isActive = false;
};