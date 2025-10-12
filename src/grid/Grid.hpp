#pragma once
#include <vector>
#include <utility>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "../utils/Point.hpp"

namespace grid
{
class Grid
{
public:
    using Point = utils::Point;

    // Constructs a triangular grid with given number of rows and spacing
    Grid(int rows, double spacing = 5.0) : rows(rows), spacing(spacing)
    {
        generateGrid();
    }

    void draw(sf::RenderWindow &window) const;

private:
    void generateGrid();
    void generateGridPoints(int numRows, int numCols);
    void generateGridLines(int numRows, int numCols);

    int rows;
    double spacing;
    std::vector<Point> gridPoints;
    sf::VertexArray lines{sf::PrimitiveType::Lines};

};
} // namespace grid