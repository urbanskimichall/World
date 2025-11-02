#pragma once
#include <vector>
#include <optional>
#include "Node.hpp"
#include "Rhombus.hpp"
#include <SFML/Graphics.hpp>

namespace grid
{
    struct GridModel
    {
        std::vector<Node> nodes;
        std::vector<Rhombus> rhombi;
        std::vector<sf::Vector2f> rhombusCenters;
        std::vector<std::vector<utils::Point>> rhombusNeighbors;
        std::optional<uint32_t> highlightedByMouseRhomusInd;
    };
}