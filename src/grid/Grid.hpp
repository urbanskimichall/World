#pragma once
#include <vector>
#include <utility>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "../utils/Point.hpp"
#include "../components/ComponentDescriptor.hpp"

namespace grid
{
class Grid
{
public:
    using Point = utils::Point;

    struct Node
    {
        Point point;
        bool isHighlighted = false;
        std::vector<Node*> neighbors;
    };

    // Constructs a triangular grid with given number of rows and spacing
    Grid(int rows, double spacing = 5.0) : rows(rows), spacing(spacing)
    {
        generateGrid();
    }

    void draw(sf::RenderWindow &window) const;
    void findPoint(const sf::Vector2f &mousePos);
    const std::vector<Node> &getGridNodes() const { return gridNodes; }
    void detectPointsOnComponent(const std::vector<components::RectComponentDescriptor> &components)
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

private:
    void generateGrid();
    void generateGridPoints(int numRows, int numCols);
    void generateGridLines(int numRows, int numCols);

    int rows;
    double spacing;
    std::vector<Node> gridNodes;
    sf::VertexArray lines{sf::PrimitiveType::Lines};
    std::optional<Point> highlightedPoint;
};
} // namespace grid