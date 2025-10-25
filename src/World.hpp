#pragma once

#include <SFML/Graphics.hpp>
#include "grid/Grid.hpp"
#include "components/Component.hpp"
#include "components/ComponentManager.hpp"
#include "grid/GridSpacing.hpp"

class World
{
public:
    void create();

private:
    void moveWorld(const sf::Vector2f &delta);
    grid::Grid grid{grid::numOfRows, grid::GRID_SPACING};
    components::ComponentManager componentManager{};

    bool isPanning = false;
    sf::Vector2f lastMousePos;
    sf::Vector2i lastMousePixel;
};