#pragma once

#include <SFML/Graphics.hpp>
#include "grid/Grid.hpp"
#include "Mover.hpp"
#include "components/ComponentManager.hpp"
#include <array>
#include "grid/GridSpacing.hpp"
#include "components/RectangleComponent.hpp"
#include "HomeTile.hpp"
#include "Tooltip.hpp"
#include "Farmer.hpp"
#include "PathManager.hpp"

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

        // componentManager.emplaceComponent<components::RectangleComponent>(grid, rectanglePoints, sf::Color::Green);
        // componentManager.emplaceComponent<components::RectangleComponent>(grid, parallerogramPoints1, sf::Color::Green);
        // componentManager.emplaceComponent<components::RectangleComponent>(grid, parallerogramPoints2, sf::Color::Green);

        updateOccupiedCells();
        // mover.init();

        const float tileWidth = 100.f;
        const float tileHeight = 50.f;
        std::string homeLevel1 = "assets/simple_home_level_1.png";
        std::string homeLevel2 = "assets/simple_home_level_2.png";
        std::string homeLevel3 = "assets/simple_home_level_3.png";
        std::string homeLevel4 = "assets/simple_home_level_4.png";
        std::string homeLevel5 = "assets/simple_home_level_5.png";
        std::string homeLevel6 = "assets/simple_home_level_6.png";
        std::string homeLevel7 = "assets/simple_home_level_7.png";
        std::string homeLevel8 = "assets/simple_home_level_8.png";
        std::string homeLevel9 = "assets/simple_home_level_9.png";
        std::string homeLevel10 = "assets/simple_home_level_10.png";
        std::string fieldLevel1 = "assets/simple_field_level_1.png";
        std::string treeLevel1 = "assets/simple_tree_level_1.png";

        std::array<sf::Vector2f, 4> parallerogramHomeLevel7 = {
            sf::Vector2f(450.f, 200.f),
            sf::Vector2f(540.f, 150.f),
            sf::Vector2f(630.f, 200.f),
            sf::Vector2f(530.f, 240.f)};
        // componentManager.emplaceComponent<components::RectangleComponent>(grid, 0, parallerogramHomeLevel7, sf::Color::Green);
        // HomeTile homeTile(grid, 0, {450, 200}, homeLevel7);
        // componentManager.emplaceComponent<HomeTile>(homeTile);
        // HomeTile homeTile2(grid, 1, {200, 50}, homeLevel1);
        // componentManager.emplaceComponent<HomeTile>(homeTile2);
        // HomeTile homeTile3(grid, 2, {250, 75}, homeLevel2);
        // componentManager.emplaceComponent<HomeTile>(homeTile3);
        // HomeTile homeTile4(grid, 3, {0, 0}, homeLevel4);
        // componentManager.emplaceComponent<HomeTile>(homeTile4);
        // HomeTile homeTile5(grid, 4, {50, 25}, homeLevel5);
        // componentManager.emplaceComponent<HomeTile>(homeTile5);
        // HomeTile homeTile6(grid, 5, {300, 50}, fieldLevel1);
        // componentManager.emplaceComponent<HomeTile>(homeTile6);
        // HomeTile homeTile7(grid, 6, {150, 75}, treeLevel1);
        // componentManager.emplaceComponent<HomeTile>(homeTile7);
        // HomeTile homeTile8(grid, 7, {600, 300}, homeLevel8);
        // componentManager.emplaceComponent<HomeTile>(homeTile8);
        // HomeTile homeTile9(grid, 8, {700, 350}, homeLevel9);
        // componentManager.emplaceComponent<HomeTile>(homeTile9);
        HomeTile homeTile10(grid, 9, {800, 400}, homeLevel10);
        componentManager.emplaceComponent<HomeTile>(homeTile10);
        HomeTile homeTile11(grid, 10, {400, 150}, homeLevel6);
        componentManager.emplaceComponent<HomeTile>(homeTile11);
        // HomeTile homeTile12(grid, 11, {350, 125}, homeLevel3);
        // componentManager.emplaceComponent<HomeTile>(homeTile12);
        // HomeTile homeTile13(grid, 12, {500, 250}, homeLevel2);
        // componentManager.emplaceComponent<HomeTile>(homeTile13);
        // HomeTile homeTile14(grid, 13, {550, 275}, homeLevel1);
        // componentManager.emplaceComponent<HomeTile>(homeTile14);
        // HomeTile homeTile15(grid, 14, {750, 375}, homeLevel5);
        // componentManager.emplaceComponent<HomeTile>(homeTile15);
        Farmer farmer1(grid, 15, homeTile10.getPosition() + sf::Vector2f{homeTile10.getSize().x / 2, homeTile10.getSize().y - 15}, homeTile10.getId());
        componentManager.emplaceComponent<Farmer>(farmer1);
        Farmer farmer2(grid, 16, homeTile11.getPosition() + sf::Vector2f{homeTile11.getSize().x / 2, homeTile11.getSize().y - 15}, homeTile11.getId());
        componentManager.emplaceComponent<Farmer>(farmer2);
    }

    void update(const sf::Vector2f &mouseWorld)
    {
        grid.highlightRhombusUnderMouse(mouseWorld);

        for (auto &component : componentManager.getComponents())
        {
            if ((static_cast<int>(component->capabilities()) & static_cast<int>(components::Capability::Movable)) != 0)
            {
                static_cast<MovableComponent &>(*component).updateMover(grid.getPath(), grid.getRhomusCentersPoints());
            }
        }

        if (const auto *c = componentManager.getHoveredComponent(mouseWorld))
        {
            tooltip.show(c->getInfo(), mouseWorld);
        }
        else
        {
            tooltip.hide();
        }
    }

    void draw(sf::RenderWindow &target) const
    {
        componentManager.draw(target);

        sf::FloatRect bounds = getViewBounds(target);
        // grid.draw(target, bounds);
        tooltip.draw(target);
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
    Tooltip tooltip;
    PathManager pathManager;
};