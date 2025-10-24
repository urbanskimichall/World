#include "Grid.hpp"
#include "GridSpacing.hpp"
#include "AreaSelector.hpp"
#include <SFML/Graphics.hpp>
#include <limits>
#include <iostream>
#include <array>
#include <algorithm>

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
        drawRhombi(window);
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

    void Grid::moveAllNodes(const sf::Vector2f &delta)
    {
        for (auto &node : gridNodes)
        {
            node.point.x += delta.x;
            node.point.y += delta.y;
        }
        for (auto &rh : rhombi)
        {
            rh.a.x += delta.x;
            rh.a.y += delta.y;
            rh.b.x += delta.x;
            rh.b.y += delta.y;
            rh.c.x += delta.x;
            rh.c.y += delta.y;
            rh.d.x += delta.x;
            rh.d.y += delta.y;
        }
    }

    void Grid::highlightRhombusUnderMouse(const sf::Vector2f &mousePos)
    {
        for (uint32_t i = 0; i < rhombi.size(); ++i)
        {
            const Rhombus &r = rhombi[i];
            if (AreaSelector::pointInConvexQuad(r, mousePos))
            {
                highlightedRhombiIndex = i;
                break;
            }
        }
    }

    void Grid::selectRhombusAtMouse(const sf::Vector2f &mousePos)
    {
        for (uint32_t i = 0; i < rhombi.size(); ++i)
        {
            const Rhombus &r = rhombi[i];
            if (AreaSelector::pointInConvexQuad(r, mousePos))
            {
                selectedRhombiIndices.push_back(i);
                break;
            }
        }
    }

    void Grid::unselectRhombusAtMouse(const sf::Vector2f &mousePos)
    {
        for (uint32_t i = 0; i < rhombi.size(); ++i)
        {
            const Rhombus &r = rhombi[i];
            if (AreaSelector::pointInConvexQuad(r, mousePos))
            {
                selectedRhombiIndices.erase(std::remove(selectedRhombiIndices.begin(), selectedRhombiIndices.end(), i), selectedRhombiIndices.end());
                break;
            }
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

        generateGridPoints(numRows, numCols, 50 * 2, 25); // Using diagX=12.0 and diagY=6.0 for hex grid
        for (auto &node : gridNodes)
        {
            std::cout << "Node at (" << node.point.x << ", " << node.point.y << ")\n";
            for (auto *neighbor : node.rightNeighbors)
            {
                std::cout << "  Right neoghbour at (" << neighbor->point.x << ", " << neighbor->point.y << ")\n";
            }
        }
        generateRhombi(numRows, numCols);

        generateGridLines(numRows, numCols);
    }

    void Grid::generateGridPoints(uint32_t numRows, uint32_t numCols,
                                  double diagX, double diagY)
    {
        gridNodes.clear();

        for (uint32_t row = 0; row < numRows; ++row)
        {
            double y = row * (diagY / 2.0);
            for (uint32_t col = 0; col < numCols; ++col)
            {
                double x = col * (diagX / 2.0) + ((row % 2) ? diagX / 4.0 : 0.0);
                gridNodes.emplace_back(Node{.point = {x, y}});
            }
        }

        // Establish neighbor connections (like before)
        auto index = [numCols](int r, int c)
        { return r * numCols + c; };

        for (uint32_t row = 0; row < numRows; ++row)
        {
            for (uint32_t col = 0; col < numCols; ++col)
            {
                Node &node = gridNodes[index(row, col)];
                node.neighbors.clear();

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

                for (auto [dr, dc] : neighborOffsets)
                {
                    int nr = static_cast<int>(row) + dr;
                    int nc = static_cast<int>(col) + dc;
                    if (nr >= 0 && nr < static_cast<int>(numRows) &&
                        nc >= 0 && nc < static_cast<int>(numCols))
                    {
                        node.neighbors.push_back(&gridNodes[index(nr, nc)]);
                    }
                }
            }
        }

        for (auto &node : gridNodes)
        {
            std::vector<Node *> rightSideNeighbors;

            for (Node *n : node.neighbors)
            {
                if (n->point.x >= node.point.x)
                    rightSideNeighbors.push_back(n);
            }

            // Optionally keep only up to 3 right-side neighbors
            if (rightSideNeighbors.size() > 3)
                rightSideNeighbors.resize(3);

            // Store them somewhere, e.g.:
            node.rightNeighbors = rightSideNeighbors;
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
    void Grid::drawRhombi(sf::RenderWindow &window) const
    {
        for (uint32_t i = 0; i < rhombi.size(); ++i)
        {
            const Rhombus &rh = rhombi[i];
            sf::ConvexShape diamond;
            diamond.setPointCount(4);
            diamond.setPoint(0, sf::Vector2f(rh.a.x, rh.a.y));
            diamond.setPoint(1, sf::Vector2f(rh.b.x, rh.b.y));
            diamond.setPoint(2, sf::Vector2f(rh.c.x, rh.c.y));
            diamond.setPoint(3, sf::Vector2f(rh.d.x, rh.d.y));

            if (i == highlightedRhombiIndex)
            {
                diamond.setFillColor(sf::Color(255, 0, 0, 50));
                diamond.setOutlineColor(sf::Color::Red);
                diamond.setOutlineThickness(2.f);
            }
            else
            {
                diamond.setFillColor(sf::Color(0, 0, 255, 30));
                diamond.setOutlineColor(sf::Color::Blue);
                diamond.setOutlineThickness(1.f);
            }
            for (const uint32_t index : selectedRhombiIndices)
            {
                if (i == index)
                {
                    diamond.setFillColor(sf::Color(0, 255, 0, 100));
                    diamond.setOutlineColor(sf::Color::Green);
                    diamond.setOutlineThickness(2.f);
                }
            }

            window.draw(diamond);
        }
    }
}