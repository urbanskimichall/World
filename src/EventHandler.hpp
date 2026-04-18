#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "Scene.hpp"
#include "ViewController.hpp"

class EventHandler
{
public:
    void handleEvent(const sf::Event &event,
                     const sf::RenderWindow &window,
                     Scene &scene,
                     ViewController &camera)
    {
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            const auto &m = *event.getIf<sf::Event::MouseButtonPressed>();
            if (m.button == sf::Mouse::Button::Left)
            {
                scene.selectRhombusAtMouse(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
            }
            if (m.button == sf::Mouse::Button::Right)
            {
                scene.unselectRhombusAtMouse(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                camera.beginPan(window);
            }
        }
        else if (event.is<sf::Event::MouseButtonReleased>())
        {
            if (event.getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Right)
            {
                camera.endPan();
            }
        }
        else if (event.is<sf::Event::MouseMoved>())
        {
            camera.updatePan(window);
        }
        else if (event.is<sf::Event::KeyPressed>())
        {
            const auto &m = *event.getIf<sf::Event::KeyPressed>();
            if (m.code == sf::Keyboard::Key::X)
            {
                // LOG_INFO("Generating paths...");
                // scene.generatePaths();
            }
            if (m.code == sf::Keyboard::Key::W)
            {
                scene.moveFormationToMouse(window);
            }
        }
        scene.handleEvent(event, window);
    }
};