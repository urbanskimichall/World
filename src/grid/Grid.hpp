#pragma once
#include "GridModel.hpp"
#include "GridGenerator.hpp"
#include "GridRenderer.hpp"
#include "GridSelector.hpp"
#include "Pathfinder.hpp"
#include "PathManager.hpp"
#include "../Logger.hpp"
#include <unordered_set>
#include <queue>
#include <SFML/Graphics.hpp>

namespace grid
{
    class Grid
    {
    public:
        Grid(uint32_t rows, uint32_t cols, double spacing = 5.0);

        void generate(uint32_t rows, uint32_t cols);
        void draw(sf::RenderWindow &window, const sf::FloatRect &viewBounds) const;

        void handleMouseClick(const sf::Vector2f &worldPos, bool toggle = true);
        void clearSelection();

        const std::vector<uint32_t> &getSelectedIndices() const { return selectedRhombi; }
        const std::vector<Node> &getGridNodes() const { return model.nodes; }
        std::vector<Node> &getGridNodes() { return model.nodes; }
        const auto &getRhomusCenters() const { return model.rhombusCenters; }
        const auto &getRhomusCentersPoints() const {return model.rhombusCentersPoints;}
        const auto &getPath() const {return model.path;}
        const Node *findClosestNode(const sf::Vector2f &pos) const { return selector.findClosestNode(model, pos); }
        uint32_t highlightRhombusUnderMouse(const sf::Vector2f &mousePos) { return selector.highlightRhombusUnderMouse(model, mousePos); }
        void selectRhombusAtMouse(const sf::Vector2f &mousePos)
        {
            const uint32_t index =
             selector.selectRhombusAtMouse(model, mousePos, selectedRhombi);

            uint32_t start = index;
            uint32_t goal = 42;

            model.path = grid::aStarFindPath(start, goal, model.rhombusCenters, model.neighborIndices, model.occupiedRhomus);
            pathManager.paths[{start,goal}] = model.path;

            if (model.path.empty())
                LOG_INFO("No path find for start index: ", start, " goal index: ", goal);
            else
            {
                LOG_INFO("Start index: ", start, " goal index: ", goal, " path length: ", model.path.size(),
                         " pathmanager paths number: ", pathManager.paths.size());
                LOG_INFO("Dist pathIndex: 0 from pathIndex: 1 DIST: ",
                    model.rhombusCentersPoints[model.path[0]].distanceTo(model.rhombusCentersPoints[model.path[1]]));
            }
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

    private:
        GridModel model;

        GridGenerator generator;
        GridRenderer renderer;
        GridSelector selector;

        PathManager pathManager;


        std::vector<uint32_t> selectedRhombi;
        uint32_t highlightedIndex = UINT32_MAX;
    };
}