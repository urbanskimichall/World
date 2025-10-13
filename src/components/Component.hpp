#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "ComponentDescriptor.hpp"
namespace components
{
    class Component
    {
    public:
        Component(float width = 90.f, float length = 45.f, sf::Color color = sf::Color::Green)
            : isDragging(false)
        {
            rectangle.setFillColor(color);
            rectangle.setPosition({200.f, 200.f});
            rectangle.setSize({width, length});
        }

        void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
        RectComponentDescriptor getDescriptor() const;
        void draw(sf::RenderTarget &target) const;

    private:
        sf::RectangleShape rectangle;
        bool isDragging;
        sf::Vector2f dragOffset;
    };
} // namespace components