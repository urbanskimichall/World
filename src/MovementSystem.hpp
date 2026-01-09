#pragma once

#include <unordered_map>
#include <vector>
#include "grid/Grid.hpp"
#include "Mover.hpp"
#include "grid/Pathfinder.hpp"

struct PathContext
{
    uint32_t currentStep;
    std::vector<uint32_t> destinationIndices;
};

class MovementSystem
{
public:
    MovementSystem(grid::Grid &grid) : grid(grid) {}

    void addMover(MovableComponent& mover, const std::vector<uint32_t>& destinationIndices)
    {
        LOG_INFO("Adding mover ID: ", mover.getId(), " to MovementSystem.");
        movers.push_back(&mover);
        pathContexts[&mover] = PathContext{0, destinationIndices}; // example destination indices
        
        setDestination(mover); // hardcoded for testing
    }

    void setDestination(MovableComponent& mover)
    {
        auto startPosition = mover.getPosition();
        auto startOpt = grid.getRhombiIndexByPosition(startPosition);
        if (!startOpt)
        {
            LOG_WARN("Mover ID: ", mover.getId(), " is not on a valid rhombus. Cannot set destination.");
            return;
        }
        const uint32_t startIndex = *startOpt;
        uint32_t goalIndex = pathContexts[&mover].destinationIndices[pathContexts[&mover].currentStep];
        goalIndex = checkIfGoalIndexIsNotOccupiedAndIfGolaIndexIsNotValidFindTheClosestFirstValid(goalIndex);
        pathContexts[&mover].currentStep = (pathContexts[&mover].currentStep + 1) % pathContexts[&mover].destinationIndices.size();
        const auto path = grid::aStarFindPath(startIndex, goalIndex, grid.getRhomusCenters(), grid.getModel().neighborIndices, grid.getModel().occupiedRhomus);

        mover.setPath(path);
    }

    uint32_t checkIfGoalIndexIsNotOccupiedAndIfGolaIndexIsNotValidFindTheClosestFirstValid(uint32_t &goalIndex)
    {
        const auto &occupiedRhomus = grid.getModel().occupiedRhomus;
        const auto &neighborIndices = grid.getModel().neighborIndices;

        if (std::find(occupiedRhomus.begin(), occupiedRhomus.end(), goalIndex) == occupiedRhomus.end())
        {
            return goalIndex;
        }

        LOG_INFO("Goal index ", goalIndex, " is occupied. Searching for closest valid index.");

        std::queue<uint32_t> toVisit;
        std::unordered_set<uint32_t> visited;
        toVisit.push(goalIndex);
        visited.insert(goalIndex);

        while (!toVisit.empty())
        {
            uint32_t current = toVisit.front();
            toVisit.pop();

            for (uint32_t neighbor : neighborIndices[current])
            {
                if (visited.find(neighbor) == visited.end())
                {
                    if (std::find(occupiedRhomus.begin(), occupiedRhomus.end(), neighbor) == occupiedRhomus.end())
                    {
                        goalIndex = neighbor;
                        return goalIndex;
                    }
                    toVisit.push(neighbor);
                    visited.insert(neighbor);
                }
            }
        }

        LOG_WARN("No valid goal index found for mover.");
        return goalIndex;
    }

    void update()
    {
        for (auto* mover : movers)
        {
            if(mover == nullptr)
            {
                LOG_WARN("Encountered null mover pointer, skipping.");
                continue;
            }
            auto step = mover->updateMover(grid.getRhomusCentersPoints());

            if (step == MovementStep::END_REACHED)
            {
                LOG_INFO("Mover ID ", mover->getId(), " reached its destination.");
                setDestination(*mover);
            }
        }
    }

private:
    grid::Grid &grid;
    std::vector<MovableComponent *> movers;
    std::unordered_map<MovableComponent *, PathContext> pathContexts;
};
