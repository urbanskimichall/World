#pragma once

#include <vector>
#include <optional>
#include <unordered_map>
#include "Node.hpp"
#include "Rhombus.hpp"
#include "Grid.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <queue>

namespace grid
{
    std::vector<uint32_t> aStarFindPath(uint32_t startIndex, uint32_t goalIndex, const Grid& grid);
}