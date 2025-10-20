#include "Component.hpp"
#include "CollisionDetection.hpp"
#include <iostream>

namespace components
{
    void Component::handleEvent(const sf::Event &event, const sf::RenderWindow &window, const std::vector<std::unique_ptr<Component>> &others)
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
            const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Vector2f newPos = mousePos + dragOffset;

            // follow grid
            const Node *closest = findClosestNode(newPos);
            if (closest)
            {
                newPos = {closest->point.x, closest->point.y};
            }
            tryMove(newPos, others);
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

    const grid::Grid::Node *Component::findClosestNode(const sf::Vector2f &position) const
    {
        const grid::Grid::Node *closestNode = nullptr;
        float minDistance = std::numeric_limits<float>::max();
        const auto gridNodes = grid.getGridNodes();
        if (gridNodes.empty())
        {
            return nullptr;
        }

        for (const auto &node : gridNodes)
        {
            const float dx = node.point.x - position.x;
            const float dy = node.point.y - position.y;
            const float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < minDistance)
            {
                minDistance = distance;
                closestNode = &node;
            }
        }
        return closestNode;
    }

    std::vector<sf::Vector2f> Component::getTransformedPoints() const
    {
        std::vector<sf::Vector2f> points;
        auto transform = rectangle.getTransform();
        sf::Vector2f size = rectangle.getSize();

        points.push_back(transform.transformPoint({0.f, 0.f}));
        points.push_back(transform.transformPoint({size.x, 0.f}));
        points.push_back(transform.transformPoint({size.x, size.y}));
        points.push_back(transform.transformPoint({0.f, size.y}));
        return points;
    }

    bool Component::tryMove(const sf::Vector2f &newPos,
                            const std::vector<std::unique_ptr<Component>> &others)
    {
        // Save previous position if rollback is needed
        sf::Vector2f prevPos = rectangle.getPosition();
        rectangle.setPosition(newPos);

        for (const auto& other : others)
        {
            if (other.get() == this)
            {
                continue;
            }

            if (convexPolygonsIntersect(getTransformedPoints(), other->getTransformedPoints()))
            {
                std::cout << "Collision detected during move!\n";
                rectangle.setPosition(prevPos);
                return false;
            }
        }
        return true;
    }
} // namespace components