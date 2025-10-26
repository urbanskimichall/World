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

        const auto pixel = sf::Mouse::getPosition(window);
        const auto deltaPixel = pixel - lastPixel;
        lastPixel = pixel;

        const float zoomFactor = view.getSize().x / window.getSize().x;
        const sf::Vector2f deltaWorld(deltaPixel.x * zoomFactor, deltaPixel.y * zoomFactor);

        view.move(-deltaWorld);
    }

private:
    sf::View &view;
    sf::Vector2i lastPixel;
    bool panning = false;
};