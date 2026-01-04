#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "ComponentDescriptor.hpp"
#include "DraggableComponent.hpp"
#include "ComponentListener.hpp"
#include "../grid/Grid.hpp"
#include "../FieldTile.hpp"

namespace components
{
    class ComponentManager : public ComponentListener
    {
    public:
        ComponentManager(grid::Grid &grid) : grid(grid) {}
        template <typename T, typename... Args>
        T &emplaceComponent(Args &&...args)
        {
            auto comp = std::make_unique<T>(std::forward<Args>(args)...);
            comp->setListener(this);
            T &ref = *comp;
            components.emplace_back(std::move(comp));
            std::cout << "ComponentManager: Emplaced component. Total components: " << components.size() << std::endl;

            return ref;
        }
        void emplaceHome(HomeTile &homeTile) { homes.push_back(homeTile); }
        void onComponentMoved() override;
        void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
        void draw(sf::RenderTarget &target) const;
        std::vector<RectComponentDescriptor> getAllDescriptors() const;
        const std::vector<std::unique_ptr<DraggableComponent>> &getComponents() const { return components; }
        
        components::DraggableComponent* getHoveredComponent(sf::Vector2f mouseWorld) const
        {
            for (auto &c : components)
            {
                if (c->contains(mouseWorld))
                    return c.get();
            }
            return nullptr;
        }

    private:
        void sortComponentsByYPosition()
        {
            std::sort(components.begin(), components.end(), [](const std::unique_ptr<DraggableComponent> &a, const std::unique_ptr<DraggableComponent> &b)
                      {
                        const auto aBounds = a->getTransformedPoints();
                        const auto bBounds = b->getTransformedPoints();
                    if (aBounds[3] != bBounds[3])
                    {
            return aBounds[3].y < bBounds[3].y;
                    }

        return aBounds[0].x < bBounds[0].x; });
        }
        std::vector<std::unique_ptr<DraggableComponent>> components;
        std::vector<HomeTile> homes;
        grid::Grid &grid;
    };
}