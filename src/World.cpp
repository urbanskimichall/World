#include "World.hpp"
#include "components/RectangleComponent.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Scene.hpp"
#include "ViewController.hpp"
#include "EventHandler.hpp"

void World::create()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 + C++23");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    window.setFramerateLimit(60);

    sf::View view = window.getDefaultView();

    Scene scene;
    scene.init();
    ViewController viewController(view);
    EventHandler eventHandler;

     while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            eventHandler.handleEvent(*event, window, scene, viewController);
        }

        scene.update(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

        window.clear();
        window.setView(view);
        scene.draw(window);
        window.display();
    }
}