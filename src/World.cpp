#include "World.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

void World::create()
{
        sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 + C++23");
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);
        window.setFramerateLimit(60);

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
                component.handleEvent(*event, window);
            }
            const sf::Vector2f mouseWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            window.clear();
            window.setView(fixedView);
            component.draw(window);
            grid.findPoint(mouseWorld);
            grid.detectPointsOnComponent(component.getDescriptor());
            grid.draw(window);
            //window.draw(shape);
            window.display();
        }
}