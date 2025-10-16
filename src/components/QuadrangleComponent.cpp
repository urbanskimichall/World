#include "QuadrangleComponent.hpp"

namespace components
{
    std::vector<sf::Vector2f> QuadrangleComponent::getTransformedPoints() const
    {
        std::vector<sf::Vector2f> transformedPoints;
        transformedPoints.reserve(QUADRANGLE_POINTS);
        const sf::Transform& transform = quadrangle.getTransform();
        for (std::size_t i = 0; i < QUADRANGLE_POINTS; ++i)
        {
            transformedPoints.push_back(transform.transformPoint(quadrangle.getPoint(i)));
        }
        return transformedPoints;
    }
}