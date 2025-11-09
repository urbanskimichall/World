#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.hpp"

class FieldTile
{
public:
    FieldTile()
        : sprite(TextureManager::instance().get("assets/field100x50.png"))
    {
    }

    void draw(sf::RenderTarget& target) const
    {
        target.draw(sprite);
    }

private:
    sf::Sprite sprite;
};