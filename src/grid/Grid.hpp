#pragma once
#include <vector>
#include <utility>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Rhombus.hpp"
#include "Node.hpp"
#include "../utils/Point.hpp"
#include "../Logger.hpp"
#include <iostream>

namespace grid
{
    class Grid
    {
    public:
        using Point = utils::Point;

        Grid(uint32_t rows, double spacing = 5.0) : rows(rows), spacing(spacing)
        {
            generateGrid();
        }

        void draw(sf::RenderWindow &window, sf::FloatRect bounds) const;
        void findPoint(const sf::Vector2f &mousePos);
        const std::vector<Node> &getGridNodes() const { return gridNodes; }
        std::vector<Node> &getGridNodes() { return gridNodes; }

        double getSpacing() const { return spacing; }
        void moveAllNodes(const sf::Vector2f &delta);
        float adjustPositionToGrid(float length) const;

        const grid::Node *findClosestNode(const sf::Vector2f &position) const
        {
            const grid::Node *closestNode = nullptr;
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
        void highlightRhombusUnderMouse(const sf::Vector2f &mousePos);
        void selectRhombusAtMouse(const sf::Vector2f &mousePos);
        void unselectRhombusAtMouse(const sf::Vector2f &mousePos);

    private:
        void generateGrid();
        void generateRhombi(uint32_t numRows, uint32_t numCols);
        void generateGridPoints(uint32_t numRows, uint32_t numCols, double diagX, double diagY);
        void generateGridLines(uint32_t numRows, uint32_t numCols);
        std::optional<Rhombus> generateSingleRhombus(const Node &node);
        std::vector<Point> generateNeighbourCenters(const Point &center, float shift);
        void drawRhombi(sf::RenderWindow &window, const sf::FloatRect &bounds) const;

        uint32_t rows;
        double spacing;
        std::vector<Node> gridNodes;
        std::vector<Rhombus> rhombi;
        std::vector<sf::Vector2f> rhombusCenters;
        std::vector<std::vector<Point>> rhombusNeighbors;
        sf::VertexArray lines{sf::PrimitiveType::Lines};
        std::optional<Point> highlightedPoint;
        uint32_t highlightedRhombiIndex{0};
        std::vector<uint32_t> selectedRhombiIndices;

        static constexpr std::array<sf::Vector2f, 8> DIRECTION_OFFSETS = {
            sf::Vector2f{0.f, -1.f},  // top
            sf::Vector2f{1.f, -0.5f}, // top-right
            sf::Vector2f{2.f, 0.f},   // right
            sf::Vector2f{1.f, 0.5f},  // bottom-right
            sf::Vector2f{0.f, 1.f},   // bottom
            sf::Vector2f{-1.f, 0.5f}, // bottom-left
            sf::Vector2f{-2.f, 0.f},  // left
            sf::Vector2f{-1.f, -0.5f} // top-left
        };
    };
} // namespace grid