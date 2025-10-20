#include "ComponentManager.hpp"
#include <iostream>

using namespace components;

void ComponentManager::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    // std::vector<Component*> componentsPtrs;
    // componentsPtrs.reserve(components.size());
    // for (auto& c : components)
    // {
    //     componentsPtrs->push_back(&c);
    // }

    for (auto &component : components)
    {
        component->handleEvent(event, window, components);
    }
}

// std::vector<RectComponentDescriptor> ComponentManager::getAllDescriptors() const
// {
//     std::vector<RectComponentDescriptor> descriptors;
//     for (const auto &comp : components)
//     {
//         descriptors.push_back(comp->get());
//     }
//     return descriptors;
// }

void ComponentManager::draw(sf::RenderTarget &target) const
{
    for (const auto &c : components)
        c->draw(target);
}

void ComponentManager::moveAllComponents(const sf::Vector2f &delta)
{
    for (auto &component : components)
    {
        const auto currentPos = component->getPosition();
        component->setPosition(sf::Vector2f{static_cast<float>(currentPos.x + delta.x), static_cast<float>(currentPos.y + delta.y)});
    }
}