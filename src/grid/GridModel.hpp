#pragma once
#include <vector>
#include <optional>
#include <unordered_map>
#include "Node.hpp"
#include "Rhombus.hpp"
#include <SFML/Graphics.hpp>

#include <unordered_set>
#include <queue>

namespace grid
{
    struct GridModel
    {
        std::vector<Node> nodes;
        std::vector<Rhombus> rhombi;
        std::vector<sf::Vector2f> rhombusCenters;
        std::vector<utils::Point> rhombusCentersPoints;
        std::vector<std::vector<utils::Point>> rhombusNeighbors;
        std::vector<std::vector<uint32_t>> neighborIndices;
        std::vector<uint32_t> occupiedRhomus;
        std::optional<uint32_t> highlightedByMouseRhomusInd;
        std::unordered_map<size_t, uint32_t> centerIndexLookup;

        std::vector<uint32_t> path;
    };
}