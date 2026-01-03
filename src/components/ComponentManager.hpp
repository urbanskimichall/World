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
        void emplaceHome(HomeTile& homeTile){homes.push_back(homeTile);}
        void onComponentMoved() override;
        void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
        void draw(sf::RenderTarget &target) const;
        std::vector<RectComponentDescriptor> getAllDescriptors() const;
        const std::vector<std::unique_ptr<DraggableComponent>> &getComponents() const { return components; }

    private:
        std::vector<std::unique_ptr<DraggableComponent>> components;
        std::vector<HomeTile> homes;
        grid::Grid &grid;
    };
}