#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.hpp"

class FieldTile
{
public:
    FieldTile(const sf::Vector2f initPosition = {0.f, 0.f})
        : sprite(TextureManager::instance().get("assets/field100x50_2.png"))
    {
        sprite.setPosition(initPosition);
    }

    void draw(sf::RenderTarget& target) const
    {
        target.draw(sprite);
    }

private:
    sf::Sprite sprite;
};