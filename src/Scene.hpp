#pragma once

#include <SFML/Graphics.hpp>
#include "grid/Grid.hpp"
#include "MovableComponent.hpp"
#include "components/ComponentManager.hpp"
#include <array>
#include "grid/GridSpacing.hpp"
#include "components/RectangleComponent.hpp"
#include "HomeTile.hpp"
#include "Tooltip.hpp"
#include "Farmer.hpp"
#include "FarmerPath.hpp"
#include "MovementSystem.hpp"
#include "LoopableMovement.hpp"
#include "TargetOrientedMovement.hpp"
#include "PathFollowingMovement.hpp"
#include "FormationFactory.hpp"
#include "ShootingSystem.hpp"
#include "SceneBoundaries.hpp"
#include "grid/GridSpacing.hpp"

class Scene
{
public:
    Scene() : grid(grid::numOfRows, grid::numOfCols), componentManager(grid), shootingSystem(grid){}

    void init()
    {
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

        // HomeTile homeTile(grid, 0, {450, 200}, homeLevel7);
        // componentManager.emplaceComponent<HomeTile>(homeTile);
        // HomeTile homeTile2(grid, 1, {200, 50}, homeLevel1);
        // componentManager.emplaceComponent<HomeTile>(homeTile2);
        // HomeTile homeTile3(grid, 2, {250, 75}, homeLevel2);
        // componentManager.emplaceComponent<HomeTile>(homeTile3);
        // HomeTile homeTile4(grid, 3, {0, 0}, homeLevel4);
        // componentManager.emplaceComponent<HomeTile>(homeTile4);
        HomeTile homeTile5(grid, 4, {50, 25}, homeLevel5);
        componentManager.emplaceComponent<HomeTile>(homeTile5);
        HomeTile homeTile6(grid, 5, {300, 50}, fieldLevel1);
        componentManager.emplaceComponent<HomeTile>(homeTile6);
        HomeTile homeTile7(grid, 6, {150, 75}, treeLevel1);
        componentManager.emplaceComponent<HomeTile>(homeTile7);
        HomeTile homeTile8(grid, 7, {600, 300}, homeLevel8);
        componentManager.emplaceComponent<HomeTile>(homeTile8);
        HomeTile homeTile9(grid, 8, {700, 350}, homeLevel9);
        componentManager.emplaceComponent<HomeTile>(homeTile9);
        HomeTile homeTile10(grid, 9, {800, 400}, homeLevel10);
        componentManager.emplaceComponent<HomeTile>(homeTile10);
        HomeTile homeTile11(grid, 10, {400, 150}, homeLevel6);
        componentManager.emplaceComponent<HomeTile>(homeTile11);
        HomeTile homeTile12(grid, 11, {350, 125}, homeLevel3);
        componentManager.emplaceComponent<HomeTile>(homeTile12);
        HomeTile homeTile13(grid, 12, {500, 250}, homeLevel2);
        componentManager.emplaceComponent<HomeTile>(homeTile13);
        HomeTile homeTile14(grid, 13, {550, 275}, homeLevel1);
        componentManager.emplaceComponent<HomeTile>(homeTile14);
        HomeTile homeTile15(grid, 14, {750, 375}, homeLevel5);
        componentManager.emplaceComponent<HomeTile>(homeTile15);
        Farmer farmer1(grid, 15, homeTile10.getPosition() + sf::Vector2f{homeTile10.getSize().x / 2, homeTile10.getSize().y + 65}, homeTile10.getId());
        auto &farmer01 = componentManager.emplaceComponent<Farmer>(farmer1);
        Farmer farmer2(grid, 16, homeTile11.getPosition() + sf::Vector2f{homeTile11.getSize().x / 2, homeTile11.getSize().y - 15}, homeTile11.getId());
        auto &farmer02 = componentManager.emplaceComponent<Farmer>(farmer2);
        Farmer farmer3(grid, 17, homeTile5.getPosition() + sf::Vector2f{homeTile5.getSize().x / 2, homeTile5.getSize().y - 15}, homeTile5.getId());
        auto &farmer03 = componentManager.emplaceComponent<Farmer>(farmer3);
        Farmer farmer4(grid, 18, homeTile9.getPosition() + sf::Vector2f{homeTile9.getSize().x / 2, homeTile9.getSize().y + 65}, homeTile9.getId());
        auto &farmer04 = componentManager.emplaceComponent<Farmer>(farmer4);

        auto farmers = formationFactory.createFormation<Farmer>(
            grid,
            3500,
            componentManager);

        int idCounter = 19; 

        for (const auto &pos : farmers)
        {
            LOG_INFO("Formation position: ", pos.x, ", ", pos.y);
            auto& shooter = componentManager.emplaceComponent<Farmer>(grid, idCounter, pos, 3500);
            shootingSystem.addShooter(shooter);
            movementSystem.addMover(shooter, std::make_unique<PathFollowingMovement<1>>(grid));
            idCounter++;
        }

        // // Add formation to manager
        // std::vector<uint32_t> unitIds;
        // std::vector<FormationOffset> offsets = DEFAULT_TROOP_PATTERN; // from FormationFactory
        // for (auto* farmer : farmers)
        // {
        //     unitIds.push_back(farmer->getId());
        //     movementSystem.addMover(*farmer, std::make_unique<PathFollowingMovement>());
        // }
        // formationMovementManager.addFormation(unitIds, 3500, offsets);

        LOG_INFO("Scene initialized.");
        // movementSystem.addMover(farmer01, {900, 3000});
        // movementSystem.addMover(farmer02, {345, 4000});

        const FarmerPath path{.homeIndex = homeTile10.getCurrentCellIndex(), .fieldIndex = homeTile6.getCurrentCellIndex(), .storageIndex = homeTile5.getCurrentCellIndex()};
        const auto destinations = FarmerPathFactory::getPathIndices(path);
        movementSystem.addMover(farmer03, std::make_unique<LoopableMovement>(grid, destinations));
        //movementSystem.addMover(farmer02, std::make_unique<LoopableMovement>(grid, destinations));
        //movementSystem.addMover(farmer01, std::make_unique<LoopableMovement>(grid, destinations));
        movementSystem.addMover(farmer04, std::make_unique<TargetOrientedMovement>(grid));

        shootingSystem.addShooter(farmer01);
        shootingSystem.addShooter(farmer02);
        shootingSystem.addShooter(farmer03);
        LOG_INFO("Movers added to MovementSystem.");

        updateOccupiedCells();
    }

    void selectRhombusAtMouse(const sf::Vector2f &mouseWorld)
    {
        grid.selectRhombusAtMouse(mouseWorld);
    }
    void unselectRhombusAtMouse(const sf::Vector2f &mouseWorld)
    {
        grid.unselectRhombusAtMouse(mouseWorld);
    }

    void update(const sf::Vector2f &mouseWorld)
    {
        grid.highlightRhombusUnderMouse(mouseWorld);

        movementSystem.update();
        shootingSystem.update();
        //formationMovementManager.update();

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
         grid.draw(target, bounds);
        shootingSystem.draw(target);
        tooltip.draw(target);

        const auto &formationFactoryPositions = formationFactory.getFormationPositions();
        for (const auto &pos : formationFactoryPositions)
        {
            sf::CircleShape circle(5.f);
            circle.setFillColor(sf::Color::Yellow);
            circle.setPosition(pos - sf::Vector2f(5.f, 5.f));
            target.draw(circle);
        }
    }

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window)
    {
        componentManager.handleEvent(event, window);
        shootingSystem.handleEvent(event, window);
    }

    void moveFormationToMouse(const sf::RenderWindow &window)
    {
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);
        // Assume formation index 0 for now
        // formationMovementManager.moveFormationTo(0, mouseWorld);
    }

    SceneBoundaries getSceneBoundaries() const
    {
        return SceneBoundaries{
            .left = 0.f,
            .right = grid::numOfCols * grid::RHOMBUS_DIAG_X / 2.f,
            .top = 0.f,
            .bottom = grid::numOfRows * grid::RHOMBUS_DIAG_Y / 2.f
        };
    }

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
        LOG_INFO("Updating occupied cells in grid...");
        grid.resetOccupiedRhombus();
        for (const auto &component : componentManager.getComponents())
        {
            for (uint32_t i = 0; i < grid.getRhomusCenters().size(); i++)
            {
                if (component->contains(grid.getRhomusCenters()[i]))
                {
                    //LOG_INFO("Added occupied index ", i);
                    grid.updateOccupiedRhombus(i);
                }
            }
        }
    }

    grid::Grid grid;
    components::ComponentManager componentManager;
    Tooltip tooltip;
    MovementSystem movementSystem;
    ShootingSystem shootingSystem;
    FormationFactory formationFactory;
};