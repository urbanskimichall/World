#include "Grid.hpp"
#include <SFML/Graphics.hpp>

void Grid::draw(sf::RenderWindow &window) const
{
    for (const auto &point : points_)
    {
        sf::CircleShape shape(2.f); // radius 2 pixels
        shape.setPosition({point.first, point.second});
        shape.setFillColor(sf::Color::Blue);
        window.draw(lines);
        window.draw(shape);
    }
}