#pragma once
#include "GridModel.hpp"
#include "AreaSelector.hpp"
#include "../Logger.hpp"

namespace grid
{
    class GridSelector
    {
    public:
        uint32_t findRhombusUnderMouse(GridModel &model, const sf::Vector2f &mousePos) const;
        void toggleSelection(std::vector<uint32_t> &selectedIndices, uint32_t index) const;
        const Node *findClosestNode(const GridModel &model, const sf::Vector2f &position) const;
        uint32_t highlightRhombusUnderMouse(GridModel &model, const sf::Vector2f &mousePos) const
        {
            for (uint32_t i = 0; i < model.rhombi.size(); ++i)
            {
                const auto &r = model.rhombi[i];
                if (AreaSelector::pointInConvexQuad(r, mousePos))
                {
                    model.highlightedByMouseRhomusInd = i;
                    return i;
                }
            }
            return UINT32_MAX;
        }

        uint32_t selectRhombusAtMouse(GridModel &model, const sf::Vector2f &mousePos,
                                      std::vector<uint32_t> &selectedRhombiIndices) const
        {
            for (uint32_t i = 0; i < model.rhombi.size(); ++i)
            {
                const Rhombus &r = model.rhombi[i];
                if (AreaSelector::pointInConvexQuad(r, mousePos))
                {
                    selectedRhombiIndices.push_back(i);
                    LOG_INFO("Rhombus at index ", i, " selected, center at (",
                             r.center().x, ", ", r.center().y, ")");
                    LOG_INFO("Neighbors count: ", model.rhombusNeighbors[i].size());
                    for (uint32_t j = 0; j < model.rhombusNeighbors[i].size(); j++)
                    {
                        LOG_INFO("Neighbour index: ", j, "position x: ", model.rhombusNeighbors[i][j].x, " y: ", model.rhombusNeighbors[i][j].y);
                    }
                    return i;
                }
            }
            return UINT32_MAX;
        }

        uint32_t unselectRhombusAtMouse(GridModel &model, const sf::Vector2f &mousePos,
                                    std::vector<uint32_t> &selectedRhombiIndices) const
        {
            for (uint32_t i = 0; i < model.rhombi.size(); ++i)
            {
                const Rhombus &r = model.rhombi[i];
                if (AreaSelector::pointInConvexQuad(r, mousePos))
                {
                    selectedRhombiIndices.erase(
                        std::remove(selectedRhombiIndices.begin(), selectedRhombiIndices.end(), i),
                        selectedRhombiIndices.end());
                    LOG_INFO("Rhombus at index ", i, " unselected, center at (",
                             r.center().x, ", ", r.center().y, ")");
                    return i;
                }
            }
            return UINT32_MAX;
        }

        std::optional<uint32_t> getRhombiIndexByPosition(const GridModel &model, const sf::Vector2f &position) const
        {
            for (uint32_t i = 0; i < model.rhombi.size(); ++i)
            {
                const Rhombus &r = model.rhombi[i];
                if (AreaSelector::pointInConvexQuad(r, position))
                {
                    return i;
                }
            }
            return std::nullopt;
        }
    };
}