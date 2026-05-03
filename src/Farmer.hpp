#pragma once

#include <SFML/Graphics.hpp>
#include "MovableComponent.hpp"
#include "components/Component.hpp"
#include "components/BoxInfo.hpp"

class Farmer : public MovableComponent
{
public:
    Farmer(grid::Grid &grid, uint16_t id, const sf::Vector2f &initPosition, uint16_t homeId)
        : MovableComponent(grid, id, homeId)
    {
        mover.setFillColor(sf::Color::Blue);
        mover.setPosition(initPosition + sf::Vector2f{10.f, 10.f});
    }

    void setPosition(const sf::Vector2f &pos) override
    {
        mover.setPosition(pos);
    }
    sf::Vector2f getPosition() const override
    {
        return mover.getPosition();
    }
    uint16_t getHomeId() const
    {
        return homeId;
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

private:
    //uint16_t homeId;
};