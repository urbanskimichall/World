#pragma once
#include "GridModel.hpp"
#include "GridGenerator.hpp"
#include "GridRenderer.hpp"
#include "GridSelector.hpp"
#include "../Logger.hpp"
#include <unordered_set>
#include <queue>
#include <SFML/Graphics.hpp>

namespace grid
{

    enum class NeighborDirection
    {
        Up,
        UpLeft,
        UpRight,
        Down,
        DownLeft,
        DownRight,
        Left,
        Right
    };

    class Grid
    {
    public:
        Grid(uint32_t rows, uint32_t cols);

        void generate(uint32_t rows, uint32_t cols);
        void draw(sf::RenderWindow &window, const sf::FloatRect &viewBounds) const;

        void handleMouseClick(const sf::Vector2f &worldPos, bool toggle = true);
        void clearSelection();

        const std::vector<uint32_t> &getSelectedIndices() const { return selectedRhombi; }
        const std::vector<Node> &getGridNodes() const { return model.nodes; }
        std::vector<Node> &getGridNodes() { return model.nodes; }
        const auto &getRhomusCenters() const { return model.rhombusCenters; }
        const auto &getRhomusCentersPoints() const { return model.rhombusCentersPoints; }
        const auto &getPath() const { return model.path; }
        const GridModel &getModel() const { return model; }
        const Node *findClosestNode(const sf::Vector2f &pos) const { return selector.findClosestNode(model, pos); }
        uint32_t highlightRhombusUnderMouse(const sf::Vector2f &mousePos)
        {
            highlightedIndex = selector.highlightRhombusUnderMouse(model, mousePos);
            return highlightedIndex;
        }

        void selectRhombusAtMouse(const sf::Vector2f &mousePos)
        {
            const uint32_t index =
                selector.selectRhombusAtMouse(model, mousePos, selectedRhombi);
        }

        void unselectRhombusAtMouse(const sf::Vector2f &mousePos)
        {
            selector.unselectRhombusAtMouse(model, mousePos, selectedRhombi);
        }
        void updateOccupiedRhombus(uint32_t index)
        {
            model.occupiedRhomus.push_back(index);
        }
        void resetOccupiedRhombus() { model.occupiedRhomus.clear(); }

        std::optional<uint32_t> getRhombiIndexByPosition(const sf::Vector2f &position) const
        {
            return selector.getRhombiIndexByPosition(model, position);
        }

        uint32_t getHighlightedIndex() const { return highlightedIndex; }

        std::optional<uint32_t> getHighlightedIndexIfNotOccupied() const
        {
            if (highlightedIndex == UINT32_MAX)
            {
                return std::nullopt;
            }
            if (std::find(model.occupiedRhomus.begin(), model.occupiedRhomus.end(), highlightedIndex) !=
                model.occupiedRhomus.end())
            {
                return std::nullopt;
            }
            return highlightedIndex;
        }

        sf::Vector2f getClosestRhombusCenterPositionToCurrentPosition(const sf::Vector2f &position) const
        {
            sf::Vector2f closestCenter;
            float minDistance = std::numeric_limits<float>::max();

            for (const auto &center : model.rhombusCenters)
            {
                float distance = std::hypot(center.x - position.x, center.y - position.y);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    closestCenter = center;
                }
            }
            return closestCenter;
        }

        // --------------------------------------------------------------------
        // Utility: return all rhombus centers that lie within a square region
        // defined by a central point and a side length.  The square is centered
        // at `squareCenter` and extends `sideLength/2` in each direction.  The
        // comparison is inclusive of the boundary.
        //
        // Parameters:
        //   squareCenter   center of the square region
        //   sideLength     length of one side of the square
        //
        // Returns:
        //   vector of positions from the grid's rhombusCenters that fall inside.
        std::vector<sf::Vector2f> getCentersInSquare(const sf::Vector2f &squareCenter,
                                                     float sideLength) const
        {
            std::vector<sf::Vector2f> result;
            float half = sideLength * 0.5f;
            float minX = squareCenter.x - half;
            float maxX = squareCenter.x + half;
            float minY = squareCenter.y - half;
            float maxY = squareCenter.y + half;

            for (const auto &c : model.rhombusCenters)
            {
                if (c.x >= minX && c.x <= maxX && c.y >= minY && c.y <= maxY)
                {
                    result.push_back(c);
                }
            }
            return result;
        }

        // --------------------------------------------------------------------
        // Utility: return all rhombus centers that lie within an axis-aligned
        // rhombus defined by its center and diagonal lengths. The rhombus has
        // diagonals of length diagX (along x-axis) and diagY (along y-axis).
        //
        // Parameters:
        //   rhombusCenter  center of the rhombus
        //   diagX          length of the x-diagonal
        //   diagY          length of the y-diagonal
        //
        // Returns:
        //   vector of positions from the grid's rhombusCenters that fall inside.
        std::vector<sf::Vector2f> getCentersInRhombus(const sf::Vector2f &rhombusCenter,
                                                      float diagX,
                                                      float diagY) const
        {
            std::vector<sf::Vector2f> result;
            float halfX = diagX * 0.5f;
            float halfY = diagY * 0.5f;

            for (const auto &c : model.rhombusCenters)
            {
                float dx = std::abs(c.x - rhombusCenter.x);
                float dy = std::abs(c.y - rhombusCenter.y);
                if (dx / halfX + dy / halfY <= 1.0f)
                {
                    result.push_back(c);
                }
            }
            return result;
        }

    private:
        GridModel model;
        GridGenerator generator;
        GridRenderer renderer;
        GridSelector selector;

        std::vector<uint32_t> selectedRhombi;
        uint32_t highlightedIndex = UINT32_MAX;
    };

    // ------------------------------------------------------------------------
    // Free utility that operates directly on a vector of center positions.
    // Returns any centers that fall inside a square region centered at
    // `squareCenter` with the given `sideLength`.
    inline std::vector<sf::Vector2f> getRhomusCentersInSquare(
        const std::vector<sf::Vector2f> &centers,
        const sf::Vector2f &squareCenter,
        float sideLength)
    {
        std::vector<sf::Vector2f> result;
        float half = sideLength * 0.5f;
        float minX = squareCenter.x - half;
        float maxX = squareCenter.x + half;
        float minY = squareCenter.y - half;
        float maxY = squareCenter.y + half;

        for (const auto &c : centers)
        {
            if (c.x >= minX && c.x <= maxX && c.y >= minY && c.y <= maxY)
            {
                result.push_back(c);
            }
        }
        return result;
    }

    // ------------------------------------------------------------------------
    // Free utility that operates directly on a vector of center positions.
    // Returns any centers that fall inside an axis-aligned rhombus defined by
    // its center point, and the lengths of its diagonals (diagX and diagY).
    // The rhombus is assumed to be axis-aligned, with diagonals along the x and y axes.
    inline std::vector<sf::Vector2f> getRhomusCentersInRhombus(
        const std::vector<sf::Vector2f> &centers,
        const sf::Vector2f &rhombusCenter,
        float diagX,
        float diagY)
    {
        std::vector<sf::Vector2f> result;
        float halfX = diagX * 0.5f;
        float halfY = diagY * 0.5f;

        for (const auto &c : centers)
        {
            float dx = std::abs(c.x - rhombusCenter.x);
            float dy = std::abs(c.y - rhombusCenter.y);
            if (dx / halfX + dy / halfY <= 1.0f)
            {
                result.push_back(c);
            }
        }
        return result;
    }
}