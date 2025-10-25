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
    Scene() : grid(grid::numOfRows, grid::GRID_SPACING) {}

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
    }

    void update(const sf::Vector2f &mouseWorld)
    {
        grid.highlightRhombusUnderMouse(mouseWorld);
    }

    void draw(sf::RenderWindow &target) const
    {
        componentManager.draw(target);
        grid.draw(target);
    }

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window)
    {
        componentManager.handleEvent(event, window);
    }

    void move(const sf::Vector2f &delta)
    {
        grid.moveAllNodes(delta);
        componentManager.moveAllComponents(delta);
    }

    grid::Grid &getGrid() { return grid; }

private:
    grid::Grid grid;
    components::ComponentManager componentManager;
};