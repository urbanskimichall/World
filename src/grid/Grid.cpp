#include "Grid.hpp"
#include <SFML/Graphics.hpp>

namespace grid
{
    void Grid::draw(sf::RenderWindow &window) const
    {
        for (const auto &point : gridPoints)
        {
            sf::CircleShape shape(2.f); // radius 2 pixels
            shape.setPosition({static_cast<float>(point.x), static_cast<float>(point.y)});
            shape.setFillColor(sf::Color::Blue);
            //window.draw(lines);
            window.draw(shape);
        }
    }

    void Grid::generateGrid()
    {
        gridPoints.clear();

        // Estimate number of rows and columns to fill a square area
        // Assume square area of (rows_ * spacing_) x (rows_ * spacing_)
        double areaSize = rows * spacing;
        int numRows = static_cast<int>(areaSize / spacing);
        int numCols = numRows;

        generateGridPoints(numRows, numCols);
        generateGridLines(numRows, numCols);
    }

    void Grid::generateGridPoints(int numRows, int numCols)
    {
        gridPoints.clear();
        for (int row = 0; row < numRows; ++row)
        {
            double y = row * spacing * std::sqrt(3) / 2.0;
            for (int col = 0; col < numCols; ++col)
            {
                double x = col * spacing + (row % 2 == 1 ? spacing / 2.0 : 0.0);
                gridPoints.emplace_back(x, y);
            }
        }
    }

    void Grid::generateGridLines(int numRows, int numCols)
    {
        lines.clear();
        for (int row = 0; row < numRows; ++row)
        {
            for (int col = 0; col < numCols; ++col)
            {
                int idx = row * numCols + col;
                if (idx >= static_cast<int>(gridPoints.size()))
                    continue;

                auto addLine = [&](int r2, int c2)
                {
                    if (r2 >= 0 && r2 < numRows && c2 >= 0 && c2 < numCols)
                    {
                        int idx2 = r2 * numCols + c2;
                        if (idx2 < static_cast<int>(gridPoints.size()))
                        {
                            lines.append(sf::Vertex(
                                sf::Vector2f(gridPoints[idx].x, gridPoints[idx].y), sf::Color::White));
                            lines.append(sf::Vertex(
                                sf::Vector2f(gridPoints[idx2].x, gridPoints[idx2].y), sf::Color::White));
                        }
                    }
                };

                // Right neighbor
                addLine(row, col + 1);

                // Row-dependent neighbors
                if (row % 2 == 0)
                {
                    addLine(row + 1, col - 1);
                    addLine(row + 1, col);
                }
                else
                {
                    addLine(row + 1, col);
                    addLine(row + 1, col + 1);
                }
            }
        }
    }

}