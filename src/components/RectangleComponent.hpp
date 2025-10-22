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
        RectangleComponent(grid::Grid &grid, std::array<sf::Vector2f, 4> points, sf::Color color)
            : DraggableComponent(grid)
        {
            std::cout << "Shape position before adjusting: (" << shape.getPosition().x << ", " << shape.getPosition().y << ")\n";

            // --- 1) Find min corner to define local space ---
            float minX = points[0].x, minY = points[0].y;
            for (const auto &p : points)
            {
                minX = std::min(minX, p.x);
                minY = std::min(minY, p.y);
            }

            // --- 2) Snap points to grid nodes ---
            std::array<sf::Vector2f, 4> localPoints;
            for (std::size_t i = 0; i < 4; ++i)
            {
                localPoints[i] = {points[i].x - minX, points[i].y - minY};
                auto node = grid.findClosestNode(localPoints[i]);
                if (node)
                {
                    localPoints[i] = {static_cast<float>(node->point.x ), static_cast<float>(node->point.y)};
                }
            }

            // --- 3) Create shape in local space ---
            shape.setPointCount(4);
            for (std::size_t i = 0; i < 4; ++i)
                shape.setPoint(static_cast<uint32_t>(i), localPoints[i]);

            shape.setFillColor(color);

            // --- 4) Snap world position to grid (based on min corner) ---
            sf::Vector2f worldPos = {minX, minY};
            if (auto node = grid.findClosestNode(worldPos))
                worldPos = {node->point.x, node->point.y};

            // --- 5) Apply world transform ---
            shape.setPosition(worldPos);

            std::cout << "Convex shape created with origin at local (0,0) and placed at world ("
                      << worldPos.x << ", " << worldPos.y << ")\n";
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