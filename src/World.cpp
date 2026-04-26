#include "World.hpp"
#include "components/RectangleComponent.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Scene.hpp"
#include "ViewController.hpp"
#include "EventHandler.hpp"
#include "FpsCounter.hpp"
#include "Logger.hpp"
#include "SceneBoundaries.hpp"

void World::create()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 + C++23");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    window.setFramerateLimit(60);

    sf::View view = window.getDefaultView();

    Scene scene;
    scene.init();
    const auto sceneBoundaries = scene.getSceneBoundaries();
    ViewController viewController(view, sceneBoundaries);
    EventHandler eventHandler;
    FpsCounter fpsCounter;
            

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            eventHandler.handleEvent(*event, window, scene, viewController);
        }
        fpsCounter.update();
        window.setTitle("Boleslaw C++23 - FPS: " + std::to_string(fpsCounter.getFps()) + "");

        scene.update(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

        window.clear(sf::Color::Green);

        window.setView(view);
        scene.draw(window);
        window.display();
    }
}