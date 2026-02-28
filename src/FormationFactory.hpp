#pragma once

#include <vector>
#include <optional>
#include <algorithm>
#include "grid/Grid.hpp"
#include "components/ComponentManager.hpp"
#include "grid/Node.hpp"

class FormationFactory
{
public:
    template <typename UnitType>
    std::vector<sf::Vector2f> createFormation(
        grid::Grid &grid,
        uint32_t centerIndex,
        components::ComponentManager &componentManager)
    {
        const auto &nodes = grid.getGridNodes();

        formationPositions = grid.getCentersInSquare({nodes[centerIndex].point.x, nodes[centerIndex].point.y}, 200.f);
        LOG_INFO("Found ", formationPositions.size(), " centers in square around center index ", centerIndex);
        return formationPositions;
    }

    const std::vector<sf::Vector2f>& getFormationPositions() const
    {
        return formationPositions;
    }

private:
    std::vector<sf::Vector2f> formationPositions{};
};