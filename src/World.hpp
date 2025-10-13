#pragma once

#include <SFML/Graphics.hpp>
#include "grid/Grid.hpp"
#include "components/Component.hpp"

class World
{
public:
    void create();

private:
    grid::Grid grid{50, 15.0};
    components::Component component;
};