#pragma once

#include "Component.hpp"
#include "../utils/Round.hpp"
#include <array>
#include "DraggableComponent.hpp"

struct QuadranglePoints
{
    std::array<sf::Vector2f, 4> points;
};

namespace components
{
    static constexpr uint8_t QUADRANGLE_POINTS = 4;
    class QuadrangleComponent : public DraggableComponent
    {
    public:
        QuadrangleComponent(const std::array<sf::Vector2f, QUADRANGLE_POINTS> &points, grid::Grid &grid, sf::Color color = sf::Color::Blue)
            : DraggableComponent(grid),
              grid(grid)
        {
            const Node *node = grid.findClosestNode(points[0]);
            sf::Vector2f snappedPos = node ? sf::Vector2f(node->point.x, node->point.y) : points[0];

            const double spacing = grid.getSpacing();
            std::array<sf::Vector2f, QUADRANGLE_POINTS> adjustedPoints = points;
            for (auto &p : adjustedPoints)
            {
                p.x = static_cast<float>(utils::roundUpToMultiple(static_cast<double>(p.x), spacing));
                p.y = static_cast<float>(utils::roundUpToMultiple(static_cast<double>(p.y), spacing));
            }

            quadrangle.setPointCount(QUADRANGLE_POINTS);
            for (std::size_t i = 0; i < QUADRANGLE_POINTS; ++i)
                quadrangle.setPoint(static_cast<uint32_t>(i), adjustedPoints[i]);

            quadrangle.setFillColor(color);

            quadrangle.setPosition(snappedPos);
        }

        void draw(sf::RenderTarget &target) const override
        {
            target.draw(quadrangle);
        }
        sf::Vector2f getPosition() const override { return quadrangle.getPosition(); }
        sf::Vector2f getSize() const
        {
            sf::FloatRect bounds = quadrangle.getLocalBounds();
            return {bounds.size.x, bounds.size.y};
        }
        void setPosition(const sf::Vector2f &pos) override { quadrangle.setPosition(pos); }
        bool contains(const sf::Vector2f &point) const override
        {
            return quadrangle.getGlobalBounds().contains(point);
        }
        std::vector<sf::Vector2f> getTransformedPoints() const override
        {
            std::vector<sf::Vector2f> pts(QUADRANGLE_POINTS);
            for (std::size_t i = 0; i < QUADRANGLE_POINTS; ++i)
                pts[i] = quadrangle.getTransform().transformPoint(quadrangle.getPoint(static_cast<uint32_t>(i)));
            return pts;
        }

    private:

        sf::ConvexShape quadrangle;
        grid::Grid &grid;
    };
}