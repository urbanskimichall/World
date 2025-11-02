#include "GridSelector.hpp"
#include <algorithm>

namespace grid
{
    uint32_t GridSelector::findRhombusUnderMouse(GridModel &model, const sf::Vector2f &mousePos) const
    {
        for (uint32_t i = 0; i < model.rhombi.size(); ++i)
        {
            if (AreaSelector::pointInConvexQuad(model.rhombi[i], mousePos))
                model.highlightedByMouseRhomusInd.value() = i;
                return i;
        }
        return UINT32_MAX;
    }

    void GridSelector::toggleSelection(std::vector<uint32_t> &selectedIndices, uint32_t index) const
    {
        auto it = std::find(selectedIndices.begin(), selectedIndices.end(), index);
        if (it != selectedIndices.end())
            selectedIndices.erase(it);
        else
            selectedIndices.push_back(index);
    }

    const Node *GridSelector::findClosestNode(const GridModel &model, const sf::Vector2f &position) const
    {
        const Node *closestNode = nullptr;
        float minDist = std::numeric_limits<float>::max();

        for (const auto &node : model.nodes)
        {
            const float dx = node.point.x - position.x;
            const float dy = node.point.y - position.y;
            const float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < minDist)
            {
                minDist = dist;
                closestNode = &node;
            }
        }

        return closestNode;
    }
}