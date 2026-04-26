#pragma once

#include "../utils/Point.hpp"
#include <vector>

namespace grid
{
    using Point = utils::Point;

    struct Node
    {
        Point point;
        std::vector<Node *> neighbors;
        std::vector<Node *> rightNeighbors;
        bool isHighlighted{false};
        bool isBoundary{false};
        double distanceOnYAxis(const Node &other) const
        {
            return std::abs(point.y - other.point.y);
        }
    };
}