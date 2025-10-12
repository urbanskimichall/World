#pragma once

#include <SFML/Graphics.hpp>
#include "grid/Grid.hpp"

class World
{
public:
    void create();

private:
    Grid grid{40, 25.0};
};