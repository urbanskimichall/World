#include "Grid.hpp"

namespace grid
{
    Grid::Grid(uint32_t rows, uint32_t cols)
    {
        generate(rows, cols);
    }

    void Grid::generate(uint32_t rows, uint32_t cols)
    {
        LOG_INFO("Generating grid ", rows, "x", cols);
        model = generator.generate(rows, cols);
        selectedRhombi.clear();
        highlightedIndex = UINT32_MAX;
    }

    void Grid::draw(sf::RenderWindow &window, const sf::FloatRect &viewBounds) const
    {
        renderer.draw(model, window, viewBounds);
        renderer.drawRhombi(model, window, viewBounds, highlightedIndex, selectedRhombi);
    }

    void Grid::handleMouseClick(const sf::Vector2f &worldPos, bool toggle)
    {
        uint32_t rhIndex = selector.findRhombusUnderMouse(model, worldPos);
        if (rhIndex == UINT32_MAX)
        {
            highlightedIndex = UINT32_MAX;
            return;
        }

        highlightedIndex = rhIndex;
        if (toggle)
            selector.toggleSelection(selectedRhombi, rhIndex);
        else
        {
            selectedRhombi.clear();
            selectedRhombi.push_back(rhIndex);
        }
    }

    void Grid::clearSelection()
    {
        selectedRhombi.clear();
        highlightedIndex = UINT32_MAX;
    }
}