#pragma once

#include <SFML/Graphics.hpp>
#include "resource/TextureManager.hpp"
#include "components/Component.hpp"
#include "components/BoxInfo.hpp"

class HomeTile : public components::Component
{
public:
    HomeTile(grid::Grid &grid, uint16_t id, sf::Vector2f initPosition = {0, 0}, std::string filename = "assets/simple_home_level_1.png")
        : Component(grid, id), sprite(TextureManager::instance().get(filename))
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
    sf::Vector2f getSize() const override
    {
        return {sprite.getGlobalBounds().size.x, sprite.getGlobalBounds().size.y};
    }

    uint32_t getCurrentCellIndex() const
    {
        sf::Vector2f center = {sprite.getPosition().x + sprite.getGlobalBounds().size.x / 2,
                               sprite.getPosition().y + (sprite.getGlobalBounds().size.y / 2) + 50.f};
        return grid.getRhombiIndexByPosition(center).value_or(UINT32_MAX);
    }

private:
    sf::Sprite sprite;
    sf::RectangleShape occupiedZone;
};