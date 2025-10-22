#include "World.hpp"
#include "components/RectangleComponent.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

void World::create()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 + C++23");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    window.setFramerateLimit(60);

    std::array<sf::Vector2f, 4> rectanglePoints = {
        sf::Vector2f(50.f, 50.f),
        sf::Vector2f(200.f, 50.f),
        sf::Vector2f(200.f, 150.f),
        sf::Vector2f(50.f, 150.f)
    };

    std::array<sf::Vector2f, 4> parallerogramPoints1 = {
        sf::Vector2f(200.f, 200.f),
        sf::Vector2f(320.f, 200.f),
        sf::Vector2f(340.f, 280.f),
        sf::Vector2f(220.f, 280.f)
    };

    std::array<sf::Vector2f, 4> parallerogramPoints2 = {
        sf::Vector2f(400.f, 200.f),
        sf::Vector2f(520.f, 200.f),
        sf::Vector2f(540.f, 280.f),
        sf::Vector2f(420.f, 280.f)
    };

    componentManager.emplaceComponent<components::RectangleComponent>(grid,rectanglePoints, sf::Color::Green);
    componentManager.emplaceComponent<components::RectangleComponent>(grid,parallerogramPoints1, sf::Color::Green);
    componentManager.emplaceComponent<components::RectangleComponent>(grid,parallerogramPoints2, sf::Color::Green);

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
        //grid.findPoint(mouseWorld);
        // for (const auto &comp : componentManager.getComponents())
        // {
        //     comp->detectPointsOnComponent(componentManager.getComponents());
        // }

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