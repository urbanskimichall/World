#pragma once
#include "GridModel.hpp"
#include "GridGenerator.hpp"
#include "GridRenderer.hpp"
#include "GridSelector.hpp"
#include "../Logger.hpp"
#include <unordered_set>
#include <queue>
#include <SFML/Graphics.hpp>

namespace grid
{
    class Grid
    {
    public:
        Grid(uint32_t rows, uint32_t cols);

        void generate(uint32_t rows, uint32_t cols);
        void draw(sf::RenderWindow &window, const sf::FloatRect &viewBounds) const;

        void handleMouseClick(const sf::Vector2f &worldPos, bool toggle = true);
        void clearSelection();

        const std::vector<uint32_t> &getSelectedIndices() const { return selectedRhombi; }
        const std::vector<Node> &getGridNodes() const { return model.nodes; }
        std::vector<Node> &getGridNodes() { return model.nodes; }
        const auto &getRhomusCenters() const { return model.rhombusCenters; }
        const auto &getRhomusCentersPoints() const { return model.rhombusCentersPoints; }
        const auto &getPath() const { return model.path; }
        const GridModel &getModel() const { return model; }
        const Node *findClosestNode(const sf::Vector2f &pos) const { return selector.findClosestNode(model, pos); }
        uint32_t highlightRhombusUnderMouse(const sf::Vector2f &mousePos)
        {
            highlightedIndex = selector.highlightRhombusUnderMouse(model, mousePos);
            return highlightedIndex;
        }

        void selectRhombusAtMouse(const sf::Vector2f &mousePos)
        {
            const uint32_t index =
                selector.selectRhombusAtMouse(model, mousePos, selectedRhombi);
        }
        void unselectRhombusAtMouse(const sf::Vector2f &mousePos)
        {
            selector.unselectRhombusAtMouse(model, mousePos, selectedRhombi);
        }
        void updateOccupiedRhombus(uint32_t index)
        {
            model.occupiedRhomus.push_back(index);
        }
        void resetOccupiedRhombus() { model.occupiedRhomus.clear(); }

        std::optional<uint32_t> getRhombiIndexByPosition(const sf::Vector2f &position) const
        {
            return selector.getRhombiIndexByPosition(model, position);
        }

        uint32_t getHighlightedIndex() const { return highlightedIndex; }

        std::optional<uint32_t> getHighlightedIndexIfNotOccupied() const
        {
            if (highlightedIndex == UINT32_MAX)
            {
                return std::nullopt;
            }
            if (std::find(model.occupiedRhomus.begin(), model.occupiedRhomus.end(), highlightedIndex) !=
                model.occupiedRhomus.end())
            {
                return std::nullopt;
            }
            return highlightedIndex;
        }

        sf::Vector2f getClosestRhombusCenterPositionToCurrentPosition(const sf::Vector2f &position) const
        {
            sf::Vector2f closestCenter;
            float minDistance = std::numeric_limits<float>::max();

            for (const auto &center : model.rhombusCenters)
            {
                float distance = std::hypot(center.x - position.x, center.y - position.y);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    closestCenter = center;
                }
            }
            return closestCenter;
        }

    private:
        GridModel model;
        GridGenerator generator;
        GridRenderer renderer;
        GridSelector selector;

        std::vector<uint32_t> selectedRhombi;
        uint32_t highlightedIndex = UINT32_MAX;
    };
}