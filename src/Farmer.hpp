#pragma once

#include <SFML/Graphics.hpp>
#include "Mover.hpp"
#include "components/DraggableComponent.hpp"
#include "components/HomeInfo.hpp"

class Farmer : public Mover, public components::DraggableComponent
{
public:
    Farmer(grid::Grid &grid, const sf::Vector2f &initPosition) : components::DraggableComponent(grid)
    {
        mover.setFillColor(sf::Color::Blue);
        mover.setPosition(initPosition);
    }

    void draw(sf::RenderTarget &target) const override
    {
        target.draw(mover);
    }
    void setPosition(const sf::Vector2f &pos) override
    {
        mover.setPosition(pos);
    }
    sf::Vector2f getPosition() const override
    {
        return mover.getPosition();
    }
    std::vector<sf::Vector2f> getTransformedPoints() const override
    {
        sf::FloatRect bounds = mover.getGlobalBounds();
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
        info.name = "Farmer";
        info.level = 0;
        info.capacity = 0;
        info.occupants = 0;
        info.happiness = 0;
        info.farmers = 1;
        info.builders = 0;
        info.soldiers = 0;
        return info;
    }
};