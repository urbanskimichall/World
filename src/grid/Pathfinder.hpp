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
    std::vector<uint32_t> aStarFindPath(
        uint32_t startIndex,
        uint32_t goalIndex,
        const std::vector<sf::Vector2f> &rhombusCenters,
        const std::vector<std::vector<uint32_t>> &neighborIndices,
        const std::vector<uint32_t> &occupiedRhomus);
}