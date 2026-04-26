#include "ViewController.hpp"

#include <algorithm>

ViewController::ViewController(sf::View &v, const SceneBoundaries &sceneBoundaries)
    : view(v), sceneBoundaries(sceneBoundaries)
{
    clampViewWithinBounds();
}

void ViewController::beginPan(const sf::RenderWindow &window)
{
    panning = true;
    lastPixel = sf::Mouse::getPosition(window);
}

void ViewController::endPan()
{
    panning = false;
}

void ViewController::updatePan(const sf::RenderWindow &window)
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
    clampViewWithinBounds();
}

void ViewController::clampViewWithinBounds()
{
    const sf::Vector2f halfSize = view.getSize() * 0.5f;

    const float minCenterX = sceneBoundaries.left + halfSize.x;
    const float maxCenterX = sceneBoundaries.right - halfSize.x;
    const float minCenterY = sceneBoundaries.top + halfSize.y;
    const float maxCenterY = sceneBoundaries.bottom - halfSize.y;

    sf::Vector2f center = view.getCenter();
    if (minCenterX <= maxCenterX)
    {
        center.x = std::clamp(center.x, minCenterX, maxCenterX);
    }
    else
    {
        center.x = (sceneBoundaries.left + sceneBoundaries.right) * 0.5f;
    }

    if (minCenterY <= maxCenterY)
    {
        center.y = std::clamp(center.y, minCenterY, maxCenterY);
    }
    else
    {
        center.y = (sceneBoundaries.top + sceneBoundaries.bottom) * 0.5f;
    }

    view.setCenter(center);
}
