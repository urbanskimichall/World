#pragma once

#include <SFML/Graphics.hpp>
#include "TextureManager.hpp"
#include "components/DraggableComponent.hpp"
#include "components/HomeInfo.hpp"

class HomeTile : public components::DraggableComponent
{
public:
    HomeTile(grid::Grid &grid, sf::Vector2f initPosition = {0, 0}, std::string filename = "assets/simple_home_level_1.png")
        : DraggableComponent(grid), sprite(TextureManager::instance().get(filename))
    {
        sprite.setPosition(initPosition);
        occupiedZone.setPosition({initPosition.x, initPosition.y + sprite.getGlobalBounds().size.y * 3 / 4});
        occupiedZone.setSize({sprite.getGlobalBounds().size.x, sprite.getGlobalBounds().size.y / 4});
    }

    void draw(sf::RenderTarget &target) const override
    {
        // target.draw(occupiedZone);
        target.draw(sprite);
    }

    sf::Vector2f getPosition() const override { return sprite.getPosition(); }
    void setPosition(const sf::Vector2f &pos) override
    {
        sprite.setPosition(pos);
        occupiedZone.setPosition({pos.x, pos.y + sprite.getGlobalBounds().size.y * 3 / 4});
    }
    std::vector<sf::Vector2f> getTransformedPoints() const override
    {
        sf::FloatRect bounds = occupiedZone.getGlobalBounds();
        std::vector<sf::Vector2f> points(4);
        points[0] = {bounds.position};
        points[1] = {bounds.position.x + bounds.size.x, bounds.position.y};
        points[2] = {bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y};
        points[3] = {bounds.position.x, bounds.position.y + bounds.size.y};
        return points;
    }
    components::HomeInfo getInfo() const override
    {
        components::HomeInfo info;
        info.name = "Simple Home";
        info.level = 1;
        info.capacity = 4;
        info.occupants = 2;
        info.happiness = 80;
        info.farmers = 1;
        info.builders = 0;
        info.soldiers = 1;
        return info;
    }

private:
    sf::Sprite sprite;
    sf::RectangleShape occupiedZone;
};