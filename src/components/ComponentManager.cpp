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
    std::vector<Component*> componentsPtrs;
    componentsPtrs.reserve(components.size());
    for (auto& c : components)
    {
        componentsPtrs.push_back(&c);
    }

    for (auto &component : components)
    {
        component.handleEvent(event, window, componentsPtrs);
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
    for (auto &component : components)
    {
        const auto currentPos = component.getDescriptor().position;
        component.setPosition({static_cast<float>(currentPos.x + delta.x), static_cast<float>(currentPos.y + delta.y)});
    }
}