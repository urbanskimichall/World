#pragma once
#include "GridModel.hpp"
#include "GridGenerator.hpp"
#include "GridRenderer.hpp"
#include "GridSelector.hpp"
#include "../Logger.hpp"
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
        const Node *findClosestNode(const sf::Vector2f &pos) const { return selector.findClosestNode(model, pos); }
        uint32_t highlightRhombusUnderMouse(const sf::Vector2f &mousePos) { return selector.highlightRhombusUnderMouse(model, mousePos); }
        void selectRhombusAtMouse(const sf::Vector2f &mousePos)
        {
            selector.selectRhombusAtMouse(model, mousePos, selectedRhombi);
        }
        void unselectRhombusAtMouse(const sf::Vector2f &mousePos)
        {
            selector.unselectRhombusAtMouse(model, mousePos, selectedRhombi);
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