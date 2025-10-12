#pragma once

#include <SFML/Graphics.hpp>
#include "grid/Grid.hpp"

class World
{
public:
    void create();

private:
    grid::Grid grid{40, 25.0};
};