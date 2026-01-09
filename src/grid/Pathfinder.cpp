#include "Pathfinder.hpp"

namespace grid
{
    std::vector<uint32_t> aStarFindPath(uint32_t startIndex, uint32_t goalIndex, const Grid &grid)
    {
        const float INF = std::numeric_limits<float>::infinity();

        const auto &rhombusCenters = grid.getRhomusCenters();
        const auto &neighborIndices = grid.getModel().neighborIndices;
        const auto &occupiedRhomus = grid.getModel().occupiedRhomus;

        size_t N = rhombusCenters.size();

        std::vector<float> gScore(N, INF);
        std::vector<float> fScore(N, INF);
        std::vector<int> cameFrom(N, -1);

        std::unordered_set<uint32_t> blocked(occupiedRhomus.begin(), occupiedRhomus.end());

        auto dist = [&](uint32_t a, uint32_t b)
        {
            sf::Vector2f da = rhombusCenters[a];
            sf::Vector2f db = rhombusCenters[b];
            return std::hypot(db.x - da.x, db.y - da.y);
        };

        auto cmp = [&](uint32_t a, uint32_t b)
        {
            return fScore[a] > fScore[b];
        };

        std::priority_queue<uint32_t, std::vector<uint32_t>, decltype(cmp)> openSet(cmp);

        gScore[startIndex] = 0;
        fScore[startIndex] = dist(startIndex, goalIndex);
        openSet.push(startIndex);

        while (!openSet.empty())
        {
            uint32_t current = openSet.top();
            openSet.pop();

            if (current == goalIndex)
            {
                // reconstruct path
                std::vector<uint32_t> path;
                for (int c = current; c != -1; c = cameFrom[c])
                    path.push_back(c);
                std::reverse(path.begin(), path.end());
                return path;
            }

            for (uint32_t neighbor : neighborIndices[current])
            {
                if (blocked.count(neighbor))
                    continue;

                float tentative = gScore[current] + dist(current, neighbor);
                if (tentative < gScore[neighbor])
                {
                    cameFrom[neighbor] = current;
                    gScore[neighbor] = tentative;
                    fScore[neighbor] = tentative + dist(neighbor, goalIndex);
                    openSet.push(neighbor);
                }
            }
        }

        return {}; // no path
    }
}