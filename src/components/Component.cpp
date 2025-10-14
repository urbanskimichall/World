#include "Component.hpp"
#include <iostream>

namespace components
{
    void Component::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
    {
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            const auto &mouse = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouse->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (rectangle.getGlobalBounds().contains(mousePos))
                {
                    isDragging = true;
                    dragOffset = rectangle.getPosition() - mousePos;
                }
            }
        }
        else if (event.is<sf::Event::MouseButtonReleased>())
        {
            const auto &mouse = event.getIf<sf::Event::MouseButtonReleased>();
            if (mouse->button == sf::Mouse::Button::Left)
            {
                isDragging = false;
                isBlocked = false;
            }
        }
        else if (event.is<sf::Event::MouseMoved>() && isDragging)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (isBlocked)
            {
                std::cout << "Movement blocked due to collision.\n";
                return;
            }

            sf::Vector2f newPos = mousePos + dragOffset;

            // Find the nearest grid node (snap)

            const Node *closest = findClosestNode(newPos);
            if (closest)
            {
                // Snap component center to node position
                sf::Vector2f size = rectangle.getSize();

                rectangle.setPosition(
                    {closest->point.x,
                     closest->point.y});
            }

            else
            {
                // No grid nodes available — fallback to free movement
                rectangle.setPosition(newPos);
            }
        }
    }

    RectComponentDescriptor Component::getDescriptor() const
    {
        return RectComponentDescriptor({rectangle.getPosition().x, rectangle.getPosition().y},
                                       rectangle.getSize().x,
                                       rectangle.getSize().y);
    }

    void Component::draw(sf::RenderTarget &target) const
    {
        target.draw(rectangle);
    }
} // namespace components