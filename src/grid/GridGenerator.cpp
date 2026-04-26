#include "GridGenerator.hpp"
#include "../Logger.hpp"
#include "GridSpacing.hpp"
#include <cmath>
#include <limits>
#include <unordered_set>
#include <queue>

namespace grid
{
    auto hashVec = [](const sf::Vector2f &v)
    {
        return (static_cast<size_t>(v.x) * 73856093) ^
               (static_cast<size_t>(v.y) * 19349663);
    };

    void buildCenterIndexLookup(GridModel &model)
    {
        model.centerIndexLookup.clear();
        for (uint32_t i = 0; i < model.rhombusCenters.size(); i++)
        {
            model.centerIndexLookup[hashVec(model.rhombusCenters[i])] = i;
        }
    }

    void buildNeighborIndices(grid::GridModel &model)
    {
        model.neighborIndices.assign(model.rhombusCenters.size(), {});

        for (uint32_t i = 0; i < model.rhombusCenters.size(); i++)
        {
            for (const auto &p : model.rhombusNeighbors[i])
            {
                sf::Vector2f v{static_cast<float>(p.x), static_cast<float>(p.y)};
                auto it = model.centerIndexLookup.find(hashVec(v));
                if (it != model.centerIndexLookup.end())
                {
                    model.neighborIndices[i].push_back(it->second);
                }
            }
        }
    }

    GridModel GridGenerator::generate(uint32_t numRows, uint32_t numCols)
    {
        GridModel model;
        generateGridPoints(model, numRows, numCols, RHOMBUS_DIAG_X, RHOMBUS_DIAG_Y);
        generateRhombi(model);
        buildCenterIndexLookup(model);
        buildNeighborIndices(model);

        return model;
    }

    void GridGenerator::generateGridPoints(GridModel &model, uint32_t numRows, uint32_t numCols,
                                           double diagX, double diagY)
    {
        model.nodes.clear();

        double minX = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();
        double minY = std::numeric_limits<double>::max();
        double maxY = std::numeric_limits<double>::lowest();

        LOG_INFO("numOfRows: ", numOfRows, " numOfCols: ", numOfCols);
        for (uint32_t row = 0; row < numRows; ++row)
        {
            const double y = row * (diagY / 2.0);
            for (uint32_t col = 0; col < numCols; ++col)
            {

                const double x = col * (diagX / 2.0) + ((row % 2) ? diagX / 4.0 : 0.0);

                minX = std::min(minX, x);
                maxX = std::max(maxX, x);
                minY = std::min(minY, y);
                maxY = std::max(maxY, y);

                const bool isBoundary =
                    (row == 0) ||
                    (row == numRows - 1) ||
                    (col == 0) ||
                    (col == numCols - 1);
                model.nodes.emplace_back(Node{
                    .point = {x, y}, .isBoundary = isBoundary});
            }
        }

        const double epsX = diagX * 0.3;
        const double epsY = diagY * 0.3;

        // for (auto &node : model.nodes)
        // {
        //     const auto &p = node.point;

        //     node.isBoundary =
        //         p.x <= minX + epsX ||
        //         p.x >= maxX - epsX ||
        //         p.y <= minY + epsY ||
        //         p.y >= maxY - epsY;
        // }

        auto index = [numCols](int r, int c)
        { return r * numCols + c; };

        for (uint32_t row = 0; row < numRows; ++row)
        {
            for (uint32_t col = 0; col < numCols; ++col)
            {
                Node &node = model.nodes[index(row, col)];
                node.neighbors.clear();

                std::vector<std::pair<int, int>> offsets = (row % 2 == 0)
                                                               ? std::vector<std::pair<int, int>>{{-1, -1}, {-1, 0}, {0, -1}, {0, 1}, {1, -1}, {1, 0}}
                                                               : std::vector<std::pair<int, int>>{{-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, 0}, {1, 1}};

                for (auto [dr, dc] : offsets)
                {
                    int nr = static_cast<int>(row) + dr;
                    int nc = static_cast<int>(col) + dc;
                    if (nr >= 0 && nr < static_cast<int>(numRows) &&
                        nc >= 0 && nc < static_cast<int>(numCols))
                    {
                        node.neighbors.push_back(&model.nodes[index(nr, nc)]);
                    }
                }

                node.rightNeighbors.clear();
                for (Node *n : node.neighbors)
                    if (n->point.x >= node.point.x)
                        node.rightNeighbors.push_back(n);

                if (node.rightNeighbors.size() > 3)
                    node.rightNeighbors.resize(3);
            }
        }
    }

    void GridGenerator::generateRhombi(GridModel &model)
    {
        model.rhombi.clear();
        model.rhombusCenters.clear();
        model.rhombusCentersPoints.clear();
        model.rhombusNeighbors.clear();
        model.neighborIndices.clear();

        for (auto &node : model.nodes)
        {
            auto maybeRh = generateSingleRhombus(node);
            if (!maybeRh)
                continue;

            Rhombus rh = *maybeRh;
            utils::Point c = rh.center();
            model.rhombi.push_back(rh);
            model.rhombusCenters.emplace_back(c.x, c.y);
            model.rhombusCentersPoints.emplace_back(utils::Point{c.x, c.y});
            constexpr float shiftToNeighbour{grid::RHOMBUS_DIAG_Y};
            model.rhombusNeighbors.push_back(generateNeighbourCenters(c, shiftToNeighbour));
        }

        float minCx = std::numeric_limits<float>::max();
        float maxCx = std::numeric_limits<float>::lowest();
        float minCy = std::numeric_limits<float>::max();
        float maxCy = std::numeric_limits<float>::lowest();

        for (const auto &center : model.rhombusCenters)
        {
            minCx = std::min(minCx, center.x);
            maxCx = std::max(maxCx, center.x);
            minCy = std::min(minCy, center.y);
            maxCy = std::max(maxCy, center.y);
        }

        constexpr float epsX = grid::RHOMBUS_DIAG_X / 24.0f;
        constexpr float epsY = grid::RHOMBUS_DIAG_Y / 24.0f;

        uint32_t index = 0;
        for (const auto &center : model.rhombusCenters)
        {
            bool rhombusBoundary =
                (center.x <= minCx + epsX) ||
                (center.x >= maxCx - epsX) ||
                (center.y <= minCy + epsY) ||
                (center.y >= maxCy - epsY);
            model.nodes[index].isBoundary = rhombusBoundary;
            ++index;
        }

        LOG_INFO("Generated ", model.rhombi.size(), " rhombi", " minCx: ", minCx, " maxCx: ", maxCx, " minCy: ", minCy, " maxCy: ", maxCy);
    }

    std::optional<Rhombus> GridGenerator::generateSingleRhombus(const Node &node)
    {
        if (node.rightNeighbors.size() < 3)
            return std::nullopt;

        Rhombus rh;
        rh.a = node.point;
        rh.b = node.rightNeighbors[0]->point;
        rh.c = node.rightNeighbors[1]->point;
        rh.d = node.rightNeighbors[2]->point;
        return rh;
    }

    std::vector<utils::Point> GridGenerator::generateNeighbourCenters(const utils::Point &c, float shift)
    {
        static constexpr std::array<sf::Vector2f, 8> OFFSETS = {
            sf::Vector2f{0.f, -1.f}, sf::Vector2f{1.f, -0.5f}, sf::Vector2f{2.f, 0.f},
            sf::Vector2f{1.f, 0.5f}, sf::Vector2f{0.f, 1.f}, sf::Vector2f{-1.f, 0.5f},
            sf::Vector2f{-2.f, 0.f}, sf::Vector2f{-1.f, -0.5f}};

        std::vector<utils::Point> neighbors;
        neighbors.reserve(8);
        for (auto &d : OFFSETS)
            neighbors.emplace_back(c.x + d.x * shift, c.y + d.y * shift);
        return neighbors;
    }
}