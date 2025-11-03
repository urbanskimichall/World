#pragma once

#include <SFML/Graphics.hpp>
#include "grid/Grid.hpp"
#include "components/ComponentManager.hpp"
#include <array>
#include "grid/GridSpacing.hpp"
#include "components/RectangleComponent.hpp"

class Scene
{
public:
    Scene() : grid(grid::numOfRows, grid::GRID_SPACING), componentManager(grid) {}

    void init()
    {
        std::array<sf::Vector2f, 4> rectanglePoints = {
            sf::Vector2f(50.f, 50.f),
            sf::Vector2f(200.f, 50.f),
            sf::Vector2f(200.f, 150.f),
            sf::Vector2f(50.f, 150.f)};

        std::array<sf::Vector2f, 4> parallerogramPoints1 = {
            sf::Vector2f(200.f, 200.f),
            sf::Vector2f(320.f, 200.f),
            sf::Vector2f(340.f, 280.f),
            sf::Vector2f(220.f, 280.f)};

        std::array<sf::Vector2f, 4> parallerogramPoints2 = {
            sf::Vector2f(400.f, 200.f),
            sf::Vector2f(520.f, 200.f),
            sf::Vector2f(540.f, 280.f),
            sf::Vector2f(420.f, 280.f)};

        componentManager.emplaceComponent<components::RectangleComponent>(grid, rectanglePoints, sf::Color::Green);
        componentManager.emplaceComponent<components::RectangleComponent>(grid, parallerogramPoints1, sf::Color::Green);
        componentManager.emplaceComponent<components::RectangleComponent>(grid, parallerogramPoints2, sf::Color::Green);

        updateOccupiedCells();
    }

    void update(const sf::Vector2f &mouseWorld)
    {
        grid.highlightRhombusUnderMouse(mouseWorld);
    }

    void draw(sf::RenderWindow &target) const
    {
        componentManager.draw(target);

        sf::FloatRect bounds = getViewBounds(target);
        grid.draw(target, bounds);
    }

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window)
    {
        componentManager.handleEvent(event, window);
    }

    grid::Grid &getGrid() { return grid; }

private:
    sf::FloatRect getViewBounds(const sf::RenderWindow &window) const
    {
        sf::View view = window.getView();
        float left = view.getCenter().x - view.getSize().x / 2.f;
        float top = view.getCenter().y - view.getSize().y / 2.f;
        float width = view.getSize().x;
        float height = view.getSize().y;
        return sf::FloatRect({left, top}, {width, height});
    }

    void updateOccupiedCells()
    {
        grid.resetOccupiedRhombus();
        for (const auto &component : componentManager.getComponents())
        {
            for (uint32_t i = 0; i < grid.getRhomusCenters().size(); i++)
            {
                if (component->contains(grid.getRhomusCenters()[i]))
                {
                    LOG_INFO("Added occupied index ", i);
                    grid.updateOccupiedRhombus(i);
                }
            }
        }
    }

    grid::Grid grid;
    components::ComponentManager componentManager;
};