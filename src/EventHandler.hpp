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
                scene.getGrid().selectRhombusAtMouse(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
            }
            if (m.button == sf::Mouse::Button::Right)
            {
                scene.getGrid().unselectRhombusAtMouse(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
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

        scene.handleEvent(event, window);
    }
};