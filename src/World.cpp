#include "World.hpp"
#include "components/RectangleComponent.hpp"
#include "components/QuadrangleComponent.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

void World::create()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 + C++23");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    window.setFramerateLimit(60);

    componentManager.emplaceComponent<components::RectangleComponent>(grid, sf::Vector2f{100.f, 50.f}, sf::Vector2f{100.f, 50.f}, sf::Color::Green);
    componentManager.emplaceComponent<components::RectangleComponent>(grid, sf::Vector2f{100.f, 250.f}, sf::Vector2f{200.f, 50.f}, sf::Color::Yellow);
    componentManager.emplaceComponent<components::RectangleComponent>(grid, sf::Vector2f{100.f, 250.f}, sf::Color::Cyan);

    sf::View fixedView(sf::FloatRect({0.f, 0.f}, {800.f, 600.f}));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    window.close();
                }
            }
            componentManager.handleEvent(*event, window);

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto &mouse = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouse->button == sf::Mouse::Button::Right)
                {
                    isPanning = true;
                    std::cout << "Started panning\n";
                    lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                }
            }
            // Stop panning
            else if (event->is<sf::Event::MouseButtonReleased>())
            {
                const auto &mouse = event->getIf<sf::Event::MouseButtonReleased>();
                if (mouse->button == sf::Mouse::Button::Right)
                    isPanning = false;
            }
            // Handle movement
            else if (event->is<sf::Event::MouseMoved>() && isPanning)
            {
                sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                sf::Vector2f delta = currentPos - lastMousePos;
                lastMousePos = currentPos;
                std::cout << "Panning by: (" << delta.x << ", " << delta.y << ")\n";

                moveWorld(delta);
            }
        }

        // Start panning

        const sf::Vector2f mouseWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        window.clear();
        window.setView(fixedView);
        componentManager.draw(window);
        grid.findPoint(mouseWorld);
        for (const auto &comp : componentManager.getComponents())
        {
            comp->detectPointsOnComponent(componentManager.getComponents());
        }

        grid.draw(window);
        // window.draw(shape);
        window.display();
    }
}

void World::moveWorld(const sf::Vector2f &delta)
{
    grid.moveAllNodes(delta);
    componentManager.moveAllComponents(delta);
}