#pragma once
#include <SFML/Graphics.hpp>
#include "Rhombus.hpp"

namespace grid
{
    class AreaSelector
    {
    public:
        static bool pointInConvexQuad(const Rhombus &r, const sf::Vector2f &p)
        {
            const sf::Vector2f a = {static_cast<float>(r.a.x), static_cast<float>(r.a.y)};
            const sf::Vector2f b = {static_cast<float>(r.b.x), static_cast<float>(r.b.y)};
            const sf::Vector2f c = {static_cast<float>(r.c.x), static_cast<float>(r.c.y)};
            const sf::Vector2f d = {static_cast<float>(r.d.x), static_cast<float>(r.d.y)};

            float c1 = cross(a, b, p);
            float c2 = cross(b, c, p);
            float c3 = cross(c, d, p);
            float c4 = cross(d, a, p);

            bool allPositive = (c1 >= 0 && c2 >= 0 && c3 >= 0 && c4 >= 0);
            bool allNegative = (c1 <= 0 && c2 <= 0 && c3 <= 0 && c4 <= 0);

            return (allPositive || allNegative);
        }

    private:
        static float cross(const sf::Vector2f &a, const sf::Vector2f &b, const sf::Vector2f &p)
        {
            return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
        }
    };
}