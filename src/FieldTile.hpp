#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.hpp"

class HomeTile
{
public:
    HomeTile(sf::Vector2f initPosition = {0,0}, std::string filename = "assets/simple_home_level_1.png")
        : sprite(TextureManager::instance().get(filename))
    {
        sprite.setPosition(initPosition);
        
        //sprite.rotate(sf::degrees(7.f));
        //sprite.scale({1.f, 0.55f});

    }

    void draw(sf::RenderTarget& target) const
    {
        target.draw(sprite);
    }

private:
    sf::Sprite sprite;
};