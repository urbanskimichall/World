#pragma once

#include "GridModel.hpp"
#include "DrawMask.hpp"
#include <SFML/Graphics.hpp>

namespace grid
{
    class GridRenderer
    {
    public:
        void draw(const GridModel &model, sf::RenderWindow &window, const sf::FloatRect &bounds) const;
        void drawRhombi(const GridModel &model, sf::RenderWindow &window, const sf::FloatRect &bounds,
                        uint32_t highlightedIndex, const std::vector<uint32_t> &selectedIndices) const;

    private:
        DrawMask drawMask{[]() {
            DrawMask mask;
            mask.set(static_cast<size_t>(DrawElement::NodePoints));
            mask.set(static_cast<size_t>(DrawElement::Rhombi));
            mask.set(static_cast<size_t>(DrawElement::HighlightedRhombi));
            mask.set(static_cast<size_t>(DrawElement::SelectedRhombi));
            mask.set(static_cast<size_t>(DrawElement::OccupiedDots));
            mask.set(static_cast<size_t>(DrawElement::BoundaryRhombi));
            return mask;
        }()};
    };
}