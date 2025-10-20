#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../grid/Grid.hpp"
#include <SFML/Window/Event.hpp>
#include "CollisionDetection.hpp"

namespace components
{
    inline bool pointInPolygon(const sf::Vector2f &p, const std::vector<sf::Vector2f> &polygon)
    {
        bool inside = false;
        std::size_t n = polygon.size();
        for (std::size_t i = 0, j = n - 1; i < n; j = i++)
        {
            const auto &pi = polygon[i];
            const auto &pj = polygon[j];

            bool intersect = ((pi.y > p.y) != (pj.y > p.y)) &&
                             (p.x < (pj.x - pi.x) * (p.y - pi.y) / (pj.y - pi.y + 1e-6f) + pi.x);
            if (intersect)
                inside = !inside;
        }
        return inside;
    }

    class DraggableComponent
    {
    public:
        explicit DraggableComponent(grid::Grid &grid) : grid(grid) {}
        virtual ~DraggableComponent() = default;

        // wspólna obsługa zdarzeń (używa virtuali poniżej)
        void handleEvent(const sf::Event &event, const sf::RenderWindow &window,
                         const std::vector<std::unique_ptr<DraggableComponent>> &others)
        {
            if (event.is<sf::Event::MouseButtonPressed>())
            {
                auto m = event.getIf<sf::Event::MouseButtonPressed>();
                if (m->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (contains(mouse))
                    {
                        isDragging = true;
                        dragOffset = getPosition() - mouse;
                    }
                }
            }
            else if (event.is<sf::Event::MouseButtonReleased>())
            {
                auto m = event.getIf<sf::Event::MouseButtonReleased>();
                if (m->button == sf::Mouse::Button::Left)
                    isDragging = false;
            }
            else if (event.is<sf::Event::MouseMoved>() && isDragging)
            {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                sf::Vector2f candidate = mouse + dragOffset;

                // snap to grid node if available
                if (auto node = grid.findClosestNode(candidate))
                    candidate = {node->point.x, node->point.y};

                tryMove(candidate, others);
            }
        }

        // próba ruchu; rollback w razie kolizji
        bool tryMove(const sf::Vector2f &newPos,
                     const std::vector<std::unique_ptr<DraggableComponent>> &others)
        {
            sf::Vector2f prev = getPosition();
            setPosition(newPos);

            for (const auto &o : others)
            {
                if (o.get() == this)
                    continue;
                if (convexPolygonsIntersect(getTransformedPoints(), o->getTransformedPoints()))
                {
                    setPosition(prev);
                    return false;
                }
            }
            return true;
        }

        virtual void detectPointsOnComponent(const std::vector<std::unique_ptr<components::DraggableComponent>> &components)
        {
            constexpr float EPSILON = grid::GRID_SPACING / 20.0f;
            for (auto &node : grid.getGridNodes())
            {
                node.isHighlighted = false;
                for (const auto &compPtr : components)
                {
                    if (!compPtr)
                        continue;
                    const auto &comp = *compPtr;
                    const auto polygon = comp.getTransformedPoints();
                    if (pointInPolygon({node.point.x, node.point.y}, polygon))
                    {
                        node.isHighlighted = true;
                        break;
                    }
                }
            }
        }

        // default contains() używa pointInPolygon na przetransformowanych wierzchołkach
        virtual bool contains(const sf::Vector2f &p) const
        {
            return pointInPolygon(p, getTransformedPoints());
        }

        // --- interface które implementuje każdy komponent ---
        virtual void draw(sf::RenderTarget &targ) const = 0;
        virtual sf::Vector2f getPosition() const = 0;
        virtual void setPosition(const sf::Vector2f &pos) = 0;
        virtual std::vector<sf::Vector2f> getTransformedPoints() const = 0;

    protected:
        grid::Grid &grid;
        bool isDragging = false;
        sf::Vector2f dragOffset;
    };
} // namespace components
