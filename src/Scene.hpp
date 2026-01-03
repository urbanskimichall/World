#pragma once

#include <SFML/Graphics.hpp>
#include "grid/Grid.hpp"
#include "Mover.hpp"
#include "components/ComponentManager.hpp"
#include <array>
#include "grid/GridSpacing.hpp"
#include "components/RectangleComponent.hpp"
#include "FieldTile.hpp"

class Scene
{
public:
    Scene() : grid(grid::numOfRows, grid::numOfCols), componentManager(grid) {}

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

        //componentManager.emplaceComponent<components::RectangleComponent>(grid, rectanglePoints, sf::Color::Green);
        //componentManager.emplaceComponent<components::RectangleComponent>(grid, parallerogramPoints1, sf::Color::Green);
        //componentManager.emplaceComponent<components::RectangleComponent>(grid, parallerogramPoints2, sf::Color::Green);

        updateOccupiedCells();
        mover.init();

        const float tileWidth = 100.f;
        const float tileHeight = 50.f;
        std::string homeLevel1 = "assets/simple_home_level_1.png";
        std::string homeLevel2 = "assets/simple_home_level_2.png";
        std::string homeLevel3 = "assets/simple_home_level_3.png";
        std::string homeLevel7 = "assets/simple_home_level_7.png";
        std::string fieldLevel1 = "assets/simple_field_level_1.png";
        std::string treeLevel1 = "assets/simple_tree_level_1.png";
        // fieldsTiles.emplace_back(HomeTile{{200, 50}, homeLevel1});
        // fieldsTiles.emplace_back(HomeTile(sf::Vector2f{250, 75}, homeLevel2));
        // fieldsTiles.emplace_back(HomeTile(sf::Vector2f{0, 0}, homeLevel1));
        // fieldsTiles.emplace_back(HomeTile({50, 25}, homeLevel3));
        // fieldsTiles.emplace_back(HomeTile({300, 50}, fieldLevel1));
        // fieldsTiles.emplace_back(HomeTile({150, 75}, treeLevel1));
        // fieldsTiles.emplace_back(HomeTile({450, 75}, homeLevel7));

        // HomeTile homeLevel7_({450, 75}, homeLevel7);

        std::array<sf::Vector2f, 4> parallerogramHomeLevel7 = {
            sf::Vector2f(450.f, 200.f),
            sf::Vector2f(540.f, 150.f),
            sf::Vector2f(630.f, 200.f),
            sf::Vector2f(530.f, 240.f)};
        componentManager.emplaceComponent<components::RectangleComponent>(grid, parallerogramHomeLevel7, sf::Color::Green);
        HomeTile homeTile(grid, {450, 200}, homeLevel7);
        componentManager.emplaceComponent<HomeTile>(homeTile);
        HomeTile homeTile2(grid, {200, 50}, homeLevel1);
        componentManager.emplaceComponent<HomeTile>(homeTile2);
    }

    void update(const sf::Vector2f &mouseWorld)
    {
        grid.highlightRhombusUnderMouse(mouseWorld);
        mover.updateMover(grid.getPath(), grid.getRhomusCentersPoints());
    }

    void draw(sf::RenderWindow &target) const
    {
        componentManager.draw(target);

        sf::FloatRect bounds = getViewBounds(target);
        grid.draw(target, bounds);

        mover.draw(target);
        // for (const auto &fieldTile : fieldsTiles)
        // {
        //     fieldTile.draw(target);
        // }
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
        std::cout << "Updating occupied rhombus cells based on component positions...\n";
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
    Mover mover;

    std::vector<HomeTile> fieldsTiles;
};