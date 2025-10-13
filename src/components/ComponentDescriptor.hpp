#pragma once
#include "../utils/Point.hpp"

namespace components
{
    using Point = utils::Point;
    struct RectComponentDescriptor
    {
        Point position; // Top-left corner position
        float width;    // Width of the rectangle
        float height;   // Height of the rectangle

        RectComponentDescriptor(Point point, float width = 0, float height = 0)
            : position(point), width(width), height(height) {}
    };
} // namespace components