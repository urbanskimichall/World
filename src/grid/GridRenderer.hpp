#pragma once
#include "GridModel.hpp"
#include <SFML/Graphics.hpp>

namespace grid
{
    class GridRenderer
    {
    public:
        void draw(const GridModel& model, sf::RenderWindow& window, const sf::FloatRect& bounds) const;
        void drawRhombi(const GridModel& model, sf::RenderWindow& window, const sf::FloatRect& bounds,
                        uint32_t highlightedIndex, const std::vector<uint32_t>& selectedIndices) const;
    };
}