#include "ComponentManager.hpp"
#include <iostream>

using namespace components;

bool ComponentManager::checkCollision(const Component &a, const Component &b) const
{
    auto da = a.getDescriptor();
    auto db = b.getDescriptor();
    return !(da.position.x + da.width < db.position.x ||
             db.position.x + db.width < da.position.x ||
             da.position.y + da.height < db.position.y ||
             db.position.y + db.height < da.position.y);
}

void ComponentManager::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    // Pass events to each component, then check for collisions
    for (auto &comp : components)
    {
        comp.handleEvent(event, window);
    }

    // 🧱 Simple collision response: just detect for now
    for (size_t i = 0; i < components.size(); ++i)
    {
        for (size_t j = i + 1; j < components.size(); ++j)
        {
            if (checkCollision(components[i], components[j]))
            {
                // Example: print or mark components
                // components[i].blockMovementOnCollision(true);
                // components[j].blockMovementOnCollision(true);
                // std::cout << "Collision detected between " << i << " and " << j << "\n";
            }
        }
    }
}

std::vector<RectComponentDescriptor> ComponentManager::getAllDescriptors() const
{
    std::vector<RectComponentDescriptor> descriptors;
    for (const auto &comp : components)
    {
        descriptors.push_back(comp.getDescriptor());
    }
    return descriptors;
}

void ComponentManager::draw(sf::RenderTarget &target) const
{
    for (const auto &c : components)
        c.draw(target);
}

void ComponentManager::moveAllComponents(const sf::Vector2f &delta)
{
    for (auto &comp : components)
    {
        const auto currentPos = comp.getDescriptor().position;
        comp.setPosition({currentPos.x + delta.x, currentPos.y + delta.y});
    }
}