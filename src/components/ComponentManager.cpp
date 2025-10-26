#include "ComponentManager.hpp"
#include <iostream>

using namespace components;

void ComponentManager::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    for (auto &component : components)
    {
        component->handleEvent(event, window, components);
    }
}

void ComponentManager::draw(sf::RenderTarget &target) const
{
    for (const auto &c : components)
        c->draw(target);
}