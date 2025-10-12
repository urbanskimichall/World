#pragma once
#include <vector>
#include <utility>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Grid
{
public:
    using Point = std::pair<double, double>;

    // Constructs a triangular grid with given number of rows and spacing
    Grid(int rows, double spacing = 5.0) : rows_(rows), spacing_(spacing)
    {
        generateGrid();
    }

    void draw(sf::RenderWindow &window) const;

private:
    int rows_;
    double spacing_;
    std::vector<Point> points_;
    const std::vector<Point> &points() const { return points_; }
    sf::VertexArray lines{sf::PrimitiveType::Lines};

    void generateGrid()
    {
        points_.clear();

        // Estimate number of rows and columns to fill a square area
        // Assume square area of (rows_ * spacing_) x (rows_ * spacing_)
        double areaSize = rows_ * spacing_;
        int numRows = static_cast<int>(areaSize / spacing_);
        int numCols = numRows;

        for (int row = 0; row < numRows; ++row)
        {
            double y = row * spacing_ * std::sqrt(3) / 2.0;
            for (int col = 0; col < numCols; ++col)
            {
                // Offset every other row for triangular grid
                double x = col * spacing_ + (row % 2 == 1 ? spacing_ / 2.0 : 0.0);
                // Only add points inside the square area
                if (x <= areaSize && y <= areaSize)
                    points_.emplace_back(x, y);
            }
        }

        for (int row = 0; row < numRows; ++row)
        {
            for (int col = 0; col < numCols; ++col)
            {
                int idx = row * numCols + col;
                if (idx >= static_cast<int>(points_.size()))
                    continue;

                auto addLine = [&](int r2, int c2)
                {
                    if (r2 >= 0 && r2 < numRows && c2 >= 0 && c2 < numCols)
                    {
                        int idx2 = r2 * numCols + c2;
                        if (idx2 < static_cast<int>(points_.size()))
                        {
                            lines.append(sf::Vertex(
                                sf::Vector2f(points_[idx].first, points_[idx].second), sf::Color::White));
                            lines.append(sf::Vertex(
                                sf::Vector2f(points_[idx2].first, points_[idx2].second), sf::Color::White));
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
};