#pragma once

#include <SFML/Graphics.hpp>
#include "grid/Grid.hpp"
#include "components/Component.hpp"
#include "components/ComponentManager.hpp"

class World
{
public:
    void create();

private:
    void moveWorld(const sf::Vector2f &delta);
    grid::Grid grid{50, 15.0};
    components::ComponentManager componentManager{};

    bool isPanning = false;
    sf::Vector2f lastMousePos;
};