#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <cmath>
#include "../grid/GridSpacing.hpp"

inline float dot(const sf::Vector2f &a, const sf::Vector2f &b)
{
    return a.x * b.x + a.y * b.y;
}

inline sf::Vector2f normalize(const sf::Vector2f &v)
{
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return len == 0 ? sf::Vector2f(0.f, 0.f) : sf::Vector2f(v.x / len, v.y / len);
}

inline void projectPolygon(const std::vector<sf::Vector2f> &pts, const sf::Vector2f &axis, float &min, float &max)
{
    min = max = dot(pts[0], axis);
    for (std::size_t i = 1; i < pts.size(); ++i)
    {
        float projection = dot(pts[i], axis);
        if (projection < min)
            min = projection;
        if (projection > max)
            max = projection;
    }
}

inline bool convexPolygonsIntersect(const std::vector<sf::Vector2f> &A,
                                    const std::vector<sf::Vector2f> &B)
{
    auto checkAxes = [&](const std::vector<sf::Vector2f> &poly1,
                         const std::vector<sf::Vector2f> &poly2) -> bool
    {
        for (std::size_t i = 0; i < poly1.size(); ++i)
        {
            sf::Vector2f edge = poly1[(i + 1) % poly1.size()] - poly1[i];
            sf::Vector2f axis = normalize({-edge.y, edge.x});

            float minA, maxA, minB, maxB;
            projectPolygon(poly1, axis, minA, maxA);
            projectPolygon(poly2, axis, minB, maxB);

            constexpr float EPSILON = -grid::GRID_SPACING / 3.f;

            if (maxA < minB - EPSILON || maxB < minA - EPSILON)
                return false;
        }
        return true;
    };

    return checkAxes(A, B) && checkAxes(B, A);
}