#pragma once

#include <SFML/Graphics.hpp>
#include "grid/Grid.hpp"

class World
{
public:
    void create();

private:
    grid::Grid grid{10, 25.0};
};