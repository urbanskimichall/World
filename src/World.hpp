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
    grid::Grid grid{50, 15.0};
    components::ComponentManager componentManager{};
};