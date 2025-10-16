#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "ComponentDescriptor.hpp"
#include "../grid/Grid.hpp"
#include "../utils/Round.hpp"
#include <iostream>

namespace components
{
    using Node = grid::Grid::Node;
    class Component
    {
    public:
        Component(const grid::Grid &grid, float xPosition = 0.f, float yPosition = 0.f, double length = 90.f, double height = 45.f, sf::Color color = sf::Color::Green)
            : grid(grid), isDragging(false)
        {
            const auto node = findClosestNode({xPosition, yPosition});
            rectangle.setFillColor(color);
            rectangle.setPosition({node->point.x, node->point.y});
            sf::Vector2f oldOrigin = rectangle.getOrigin();
            sf::Vector2f oldPos = rectangle.getPosition();
            sf::Vector2f newOrigin = {0.f, rectangle.getSize().y};
            sf::Vector2f offset = newOrigin - oldOrigin;
            rectangle.setOrigin(newOrigin);
            rectangle.setPosition(oldPos + offset);
            length = utils::roundUpToMultiple(length, (grid.getSpacing()));
            const auto distanceToRightNeighbor = node->distanceOnYAxis(*node->neighbors[1]);
            height = utils::roundUpToMultiple(height, (distanceToRightNeighbor));
            rectangle.setSize({static_cast<float>(length), static_cast<float>(height)});
        }

        void handleEvent(const sf::Event &event, const sf::RenderWindow &window, const std::vector<Component *> &others);
        RectComponentDescriptor getDescriptor() const;
        void draw(sf::RenderTarget &target) const;
        void blockMovementOnCollision(bool block) { isBlocked = block; }
        const grid::Grid::Node *findClosestNode(const sf::Vector2f &position) const;
        void setPosition(const sf::Vector2f &position) { rectangle.setPosition(position); }

    private:
        std::vector<sf::Vector2f> getTransformedPoints() const;
        bool tryMove(const sf::Vector2f& newPos, const std::vector<Component*>& others);
        const grid::Grid &grid;
        sf::RectangleShape rectangle;
        bool isDragging;
        sf::Vector2f dragOffset;
        bool isBlocked = false;
    };
} // namespace components