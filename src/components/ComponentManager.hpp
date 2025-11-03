#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "ComponentDescriptor.hpp"
#include "DraggableComponent.hpp"
#include "ComponentListener.hpp"
#include "../grid/Grid.hpp"

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

            return ref;
        }
        void onComponentMoved() override;
        void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
        void draw(sf::RenderTarget &target) const;
        std::vector<RectComponentDescriptor> getAllDescriptors() const;
        const std::vector<std::unique_ptr<DraggableComponent>> &getComponents() const { return components; }

    private:
        std::vector<std::unique_ptr<DraggableComponent>> components;
        grid::Grid &grid;
    };
}