#include "ComponentManager.hpp"
#include <iostream>

using namespace components;

void ComponentManager::onComponentMoved()
{
    grid.resetOccupiedRhombus(); // optional: clear previous markings

    for (const auto &component : components)
    {
        for (uint32_t i = 0; i < grid.getRhomusCenters().size(); i++)
        {
            if (component->contains(grid.getRhomusCenters()[i]))
            {
                grid.updateOccupiedRhombus(i);
            }
        }
    }

    LOG_INFO("Updated occupied rhombi for moved component.");
}

void ComponentManager::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    for (auto &component : components)
    {
        component->handleEvent(event, window, components);
    }
}

void ComponentManager::draw(sf::RenderTarget &target) const
{
    for (uint16_t i{0}; i < components.size(); i++)
    {
        components[i]->draw(target);
        //homes[i].draw(target);
    }
        
}