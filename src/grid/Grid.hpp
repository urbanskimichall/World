#pragma once
#include <vector>
#include <utility>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "../utils/Point.hpp"

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
            std::vector<Node *> neighbors;
            double distanceOnYAxis(const Node &other) const
            {
                return std::abs(point.y - other.point.y);
            }
        };

        Grid(uint32_t rows, double spacing = 5.0) : rows(rows), spacing(spacing)
        {
            generateGrid();
        }

        void draw(sf::RenderWindow &window) const;
        void findPoint(const sf::Vector2f &mousePos);
        const std::vector<Node> &getGridNodes() const { return gridNodes; }
        std::vector<Node> &getGridNodes() { return gridNodes; }

        double getSpacing() const { return spacing; }
        void moveAllNodes(const sf::Vector2f &delta);
        float adjustPositionToGrid(float length) const;

        const grid::Grid::Node *findClosestNode(const sf::Vector2f &position) const
        {
            const grid::Grid::Node *closestNode = nullptr;
            float minDistance = std::numeric_limits<float>::max();
            if (gridNodes.empty())
            {
                return nullptr;
            }

            for (const auto &node : gridNodes)
            {
                const float dx = node.point.x - position.x;
                const float dy = node.point.y - position.y;
                const float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < minDistance)
                {
                    minDistance = distance;
                    closestNode = &node;
                }
            }
            return closestNode;
        }

    private:
        void generateGrid();
        void generateGridPoints(uint32_t numRows, uint32_t numCols);
        void generateGridLines(uint32_t numRows, uint32_t numCols);

        uint32_t rows;
        double spacing;
        std::vector<Node> gridNodes;
        sf::VertexArray lines{sf::PrimitiveType::Lines};
        std::optional<Point> highlightedPoint;
    };
} // namespace grid