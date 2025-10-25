#pragma once

#include "../utils/Point.hpp"
#include <vector>

namespace grid
{
    using Point = utils::Point;

    struct Node
    {
        Point point;
        bool isHighlighted = false;
        std::vector<Node *> neighbors;
        std::vector<Node *> rightNeighbors;
        double distanceOnYAxis(const Node &other) const
        {
            return std::abs(point.y - other.point.y);
        }
    };
}