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
    };
}