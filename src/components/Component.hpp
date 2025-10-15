#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "ComponentDescriptor.hpp"
#include "../grid/Grid.hpp"

namespace components
{
    using Node = grid::Grid::Node;
    class Component
    {
    public:
        Component(const grid::Grid &grid, float xPosition = 0.f, float yPosition = 0.f, float length = 90.f, float height = 45.f, sf::Color color = sf::Color::Green)
            : grid(grid), isDragging(false)
        {
            const auto node = findClosestNode({xPosition, yPosition});
            rectangle.setFillColor(color);
            rectangle.setPosition({node->point.x, node->point.y});
            length = grid.adjustPositionToGrid(length);
            height = grid.adjustPositionToGrid(height);
            rectangle.setSize({length, height});
        }

        void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
        RectComponentDescriptor getDescriptor() const;
        void draw(sf::RenderTarget &target) const;
        void blockMovementOnCollision(bool block) { isBlocked = block; }
        const grid::Grid::Node *findClosestNode(const sf::Vector2f &position) const;
        void setPosition(const sf::Vector2f &position) { rectangle.setPosition(position); }

    private:
        const grid::Grid &grid;
        sf::RectangleShape rectangle;
        bool isDragging;
        sf::Vector2f dragOffset;
        bool isBlocked = false;
    };
} // namespace components