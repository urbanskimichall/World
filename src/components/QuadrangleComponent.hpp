#pragma once

#include "Component.hpp"

namespace components
{
    static constexpr uint8_t QUADRANGLE_POINTS = 4;
    class QuadrangleComponent : public Component
    {
    public:
        QuadrangleComponent(const std::array<sf::Vector2f, QUADRANGLE_POINTS>& points, const grid::Grid& grid, sf::Color color = sf::Color::Blue)
            : Component(grid, 0.f, 0.f, 100.f, 50.f, color) // Placeholder values
            , grid(grid)
        {
           
            quadrangle.setPointCount(QUADRANGLE_POINTS);
            for (std::size_t i = 0; i < QUADRANGLE_POINTS; ++i)
            {
                quadrangle.setPoint(static_cast<uint32_t>(i), points[i]);
            }
            quadrangle.setFillColor(color);
        }

        private:
        std::vector<sf::Vector2f> getTransformedPoints() const;
        sf::ConvexShape quadrangle;
        const grid::Grid& grid;
    };
}