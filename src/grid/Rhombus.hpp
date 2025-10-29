#pragma once

#include "../utils/Point.hpp"

namespace grid
{
    using Point = utils::Point;
    struct Rhombus
    {
        Point a;
        Point b;
        Point c;
        Point d;
        Point center() const
        {
            return Point((a.x + b.x + c.x + d.x) / 4.0, (a.y + b.y + c.y + d.y) / 4.0);
        }
        std::vector<Point*> neighboursCenters;
    };
}