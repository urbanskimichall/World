#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "ComponentDescriptor.hpp"
#include "DraggableComponent.hpp"
#include "../grid/Grid.hpp"
#include "../utils/Round.hpp"

namespace components
{
    class RectangleComponent : public DraggableComponent
    {
    public:
        RectangleComponent(grid::Grid &grid, sf::Vector2f size, sf::Vector2f position, sf::Color color)
            : DraggableComponent(grid)
        {
            shape.setPointCount(4);
            shape.setPoint(0, {0.f, 0.f});
            shape.setPoint(1, {size.x, 0.f});
            shape.setPoint(2, {size.x, size.y});
            shape.setPoint(3, {0.f, size.y});
            shape.setFillColor(color);

            setOriginToBottomLeft();
            if (auto node = grid.findClosestNode(position))
            {
                setPosition({node->point.x, node->point.y});
            }
            else
            {
                std::cout<<"No node found for position (" << position.x << ", " << position.y << ")\n";
                setPosition(position);
            }
        }
        RectangleComponent(grid::Grid &grid, sf::Vector2f position, sf::Color color)
            : DraggableComponent(grid)
        {
            shape.setPointCount(4);
            shape.setPoint(0, {0.f, 0.f});
            shape.setPoint(1, {120.f, 0.f});
            shape.setPoint(2, {140.f, 80.f});
            shape.setPoint(3, {20.f, 80.f});
            shape.setFillColor(color);

            setOriginToBottomLeft();
            if (auto node = grid.findClosestNode(position))
            {
                setPosition({node->point.x, node->point.y});
            }
            else
            {
                std::cout<<"No node found for position (" << position.x << ", " << position.y << ")\n";
                setPosition(position);
            }
        }

        void draw(sf::RenderTarget &target) const override { target.draw(shape); }

        sf::Vector2f getPosition() const override { return shape.getPosition(); }
        void setPosition(const sf::Vector2f &pos) override { shape.setPosition(pos); }

        std::vector<sf::Vector2f> getTransformedPoints() const override
        {
            std::vector<sf::Vector2f> pts(shape.getPointCount());
            for (std::size_t i = 0; i < pts.size(); ++i)
                pts[i] = shape.getTransform().transformPoint(shape.getPoint(static_cast<uint32_t>(i)));
            return pts;
        }

    private:
        sf::ConvexShape shape;

        void setOriginToBottomLeft()
        {
            auto b = shape.getLocalBounds(); // left, top, width, height
            sf::Vector2f newOrigin(b.position.x, b.position.y + b.size.y);
            // zachowaj pozycję wizualną:
            sf::Vector2f oldPos = shape.getPosition();
            shape.setOrigin(newOrigin);
            shape.setPosition(oldPos + (newOrigin - shape.getOrigin()));
        }
    };
}