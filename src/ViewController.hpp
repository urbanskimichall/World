#pragma once

#include <SFML/Graphics.hpp>

class ViewController
{
public:
    explicit ViewController(sf::View &v) : view(v) {}

    void beginPan(const sf::RenderWindow &window)
    {
        panning = true;
        lastPixel = sf::Mouse::getPosition(window);
    }

    void endPan() { panning = false; }

    void updatePan(const sf::RenderWindow &window)
    {
        if (!panning)
        {
            return;
        }

        auto pixel = sf::Mouse::getPosition(window);
        auto deltaPixel = pixel - lastPixel;
        lastPixel = pixel;

        float zoomFactor = view.getSize().x / window.getSize().x;
        sf::Vector2f deltaWorld(deltaPixel.x * zoomFactor, deltaPixel.y * zoomFactor);

        view.move(-deltaWorld);
    }

private:
    sf::View &view;
    bool panning = false;
    sf::Vector2i lastPixel;
};