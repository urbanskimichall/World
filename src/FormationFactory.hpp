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

        formationPositions = grid.getCentersInRhombus({static_cast<float>(nodes[centerIndex].point.x), static_cast<float>(nodes[centerIndex].point.y)}, 200.f, 100.f);
        LOG_INFO("Found ", formationPositions.size(), " centers in rhombus around center index ", centerIndex);
        return formationPositions;
    }

    const std::vector<sf::Vector2f>& getFormationPositions() const
    {
        return formationPositions;
    }

    std::vector<sf::Vector2f>& getFormationPositions()
    {
        return formationPositions;
    }

private:
    std::vector<sf::Vector2f> formationPositions{};
};