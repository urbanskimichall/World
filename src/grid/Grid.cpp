#include "Grid.hpp"
#include <SFML/Graphics.hpp>
#include <limits>
#include <iostream>
#include <array>

namespace grid
{
    void Grid::draw(sf::RenderWindow &window) const
    {
        for (const auto &node : gridNodes)
        {
            sf::CircleShape shape(2.f); // radius 2 pixels
            shape.setPosition({static_cast<float>(node.point.x), static_cast<float>(node.point.y)});
            shape.setFillColor(sf::Color::Blue);
            if (node.isHighlighted)
            {
                shape.setFillColor(sf::Color::Red);
            }
            // window.draw(lines);
            window.draw(shape);
        }
    }

    void Grid::findPoint(const sf::Vector2f &mousePos)
    {
        utils::Point mousePoint(mousePos.x, mousePos.y);
        double minDist = std::numeric_limits<double>::max();
        std::array<std::pair<Point, double>, 4> dists{{{Point(0, 0), minDist}, {Point(0, 0), minDist}, {Point(0, 0), minDist}, {Point(0, 0), minDist}}};

        for (auto &node : gridNodes)
        {
            node.isHighlighted = false;
            double dist = node.point.distanceTo(mousePoint);
            for (auto &[p, d] : dists)
            {
                if (dist < d)
                {
                    d = dist;
                    p = node.point;
                    break;
                }
            }
            if (dist < minDist)
            {
                minDist = dist;
                highlightedPoint = node.point;
            }
        }

        {
            // std::cout << "Point: (" << p.x << ", " << p.y << ") Distance: " << d << "\n";
            std::find_if(gridNodes.begin(), gridNodes.end(), [this](const Node &n)
                         { return n.point == *highlightedPoint; })
                ->isHighlighted = true;
        }
    }

    void Grid::detectPointsOnComponent(const std::vector<components::RectComponentDescriptor> &components)
    {
        for (auto &node : gridNodes)
        {
            node.isHighlighted = false;
            for (const auto &comp : components)
            {
                if (node.point.x >= comp.position.x && node.point.x <= comp.position.x + comp.width &&
                    node.point.y >= comp.position.y && node.point.y <= comp.position.y + comp.height)
                {
                    node.isHighlighted = true;
                }
            }
        }
    }

    void Grid::moveAllNodes(const sf::Vector2f &delta)
    {
        for (auto &node : gridNodes)
        {
            node.point.x += delta.x;
            node.point.y += delta.y;
        }
    }

    float Grid::adjustPositionToGrid(float length) const
    {
        return length / spacing * gridNodes.front().neighbors.front()->point.distanceTo(gridNodes.front().point);
    }

    void Grid::generateGrid()
    {
        gridNodes.clear();

        // Estimate number of rows and columns to fill a square area
        // Assume square area of (rows_ * spacing_) x (rows_ * spacing_)
        double areaSize = rows * spacing;
        int numRows = static_cast<int>(areaSize / spacing);
        int numCols = numRows;

        generateGridPoints(numRows, numCols);
        for(auto &node : gridNodes) {
            std::cout << "Node at (" << node.point.x << ", " << node.point.y << ")\n";
        }

        generateGridLines(numRows, numCols);
    }

    void Grid::generateGridPoints(uint32_t numRows, uint32_t numCols)
    {
        gridNodes.clear();
        for (uint32_t row = 0; row < numRows; ++row)
        {
            double y = row * spacing * std::sqrt(3) / 2.0;
            for (uint32_t col = 0; col < numCols; ++col)
            {
                double x = col * spacing + (row % 2 == 1 ? spacing / 2.0 : 0.0);
                gridNodes.emplace_back(Node{.point = {x, y}});
            }
        }

        // to be refactored
        //  Establish neighbors for each node in the triangular grid

        auto index = [numCols](int r, int c)
        { return r * numCols + c; };

        for (uint32_t row = 0; row < numRows; ++row)
        {
            for (uint32_t col = 0; col < numCols; ++col)
            {
                Node &node = gridNodes[index(row, col)];

                // Offsets depend on whether the row is even or odd
                std::vector<std::pair<int, int>> neighborOffsets;
                if (row % 2 == 0)
                {
                    neighborOffsets = {
                        {-1, -1}, {-1, 0}, {0, -1}, {0, 1}, {1, -1}, {1, 0}};
                }
                else
                {
                    neighborOffsets = {
                        {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, 0}, {1, 1}};
                }

                // Add valid neighbors
                for (auto [dr, dc] : neighborOffsets)
                {
                    int nr = row + dr;
                    int nc = col + dc;

                    if (nr >= 0 && nr < numRows && nc >= 0 && nc < numCols)
                        node.neighbors.push_back(&gridNodes[index(nr, nc)]);
                }
            }
        }
    }

    void Grid::generateGridLines(uint32_t numRows, uint32_t numCols)
    {
        lines.clear();
        for (uint32_t row = 0; row < numRows; ++row)
        {
            for (uint32_t col = 0; col < numCols; ++col)
            {
                uint32_t idx = row * numCols + col;
                if (idx >= static_cast<uint32_t>(gridNodes.size()))
                    continue;

                auto addLine = [&](uint32_t r2, uint32_t c2)
                {
                    if (r2 >= 0 && r2 < numRows && c2 >= 0 && c2 < numCols)
                    {
                        uint32_t idx2 = r2 * numCols + c2;
                        if (idx2 < static_cast<uint32_t>(gridNodes.size()))
                        {
                            lines.append(sf::Vertex(
                                sf::Vector2f(gridNodes[idx].point.x, gridNodes[idx].point.y), sf::Color::White));
                            lines.append(sf::Vertex(
                                sf::Vector2f(gridNodes[idx2].point.x, gridNodes[idx2].point.y), sf::Color::White));
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