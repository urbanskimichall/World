#pragma once
#include <vector>
#include <utility>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Rhombus.hpp"
#include "../utils/Point.hpp"
#include <iostream>

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
            std::vector<Node *> rightNeighbors;
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

        void generateRhombi(uint32_t numRows, uint32_t numCols)
        {
            rhombi.clear();

            for (const auto &node : gridNodes)
            {
                if (node.rightNeighbors.size() < 3)
                {
                    continue;
                }
                if (node.rightNeighbors.size() > 4)
                {
                    std::cerr << "Warning: Node has more than 4 right neighbors, skipping rhombus generation for this node.\n";
                }

                Rhombus rh;
                rh.a = node.point;
                rh.b = node.rightNeighbors[0]->point;
                rh.c = node.rightNeighbors[1]->point;
                rh.d = node.rightNeighbors[2]->point;
                rhombi.push_back(rh);
            }
        }
        void drawRhombi(sf::RenderWindow &window) const;
        void highlightRhombusUnderMouse(const sf::Vector2f &mousePos);
        void selectRhombusAtMouse(const sf::Vector2f &mousePos);

    private:
        void generateGrid();
        void generateGridPoints(uint32_t numRows, uint32_t numCols, double diagX, double diagY);
        void generateGridLines(uint32_t numRows, uint32_t numCols);

        uint32_t rows;
        double spacing;
        std::vector<Node> gridNodes;
        std::vector<Rhombus> rhombi;
        sf::VertexArray lines{sf::PrimitiveType::Lines};
        std::optional<Point> highlightedPoint;
        uint32_t highlightedRhombiIndex{0};
        std::vector<uint32_t> selectedRhombiIndices;
    };
} // namespace grid