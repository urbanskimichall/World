#pragma once

#include <SFML/Graphics.hpp>
#include "SceneBoundaries.hpp"

class ViewController
{
public:
    explicit ViewController(sf::View &v, const SceneBoundaries &sceneBoundaries);

    void beginPan(const sf::RenderWindow &window);
    void endPan();
    void updatePan(const sf::RenderWindow &window);

private:
    void clampViewWithinBounds();

    sf::View &view;
    const SceneBoundaries sceneBoundaries;
    sf::Vector2i lastPixel;
    bool panning = false;
};