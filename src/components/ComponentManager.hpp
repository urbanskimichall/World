#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "ComponentDescriptor.hpp"

namespace components
{
    class ComponentManager
    {
    public:
        void addComponent(const Component& comp) { components.push_back(comp); }
        void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
        void draw(sf::RenderTarget& target) const;
        std::vector<RectComponentDescriptor> getAllDescriptors() const;
        void moveAllComponents(const sf::Vector2f &delta);

    private:
        std::vector<Component> components;
        bool checkCollision(const Component& moving, const Component& other) const;
    };
}