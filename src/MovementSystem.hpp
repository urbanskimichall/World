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
    MovementSystem(const grid::Grid &grid) : grid(grid) {}

    void addMover(MovableComponent &mover, const std::vector<uint32_t> &destinationIndices)
    {
        LOG_INFO("Adding mover ID: ", mover.getId(), " to MovementSystem.");
        movers.push_back(&mover);

        const std::vector<uint32_t> validDestinations = [&]()
        {
            std::vector<uint32_t> validIndices;
            for (const auto index : destinationIndices)
            {
                if (index >= grid.getRhomusCenters().size())
                {
                    LOG_WARN("Destination index ", index, " is out of bounds. Skipping.");
                    continue;
                }
                validIndices.push_back(index);
            }
            return validIndices;
        }();

        pathContexts[&mover] = PathContext{0, validDestinations};

        setDestination(mover);
    }

    void update()
    {
        for (auto *mover : movers)
        {
            if (mover == nullptr)
            {
                LOG_WARN("Encountered null mover pointer, skipping.");
                continue;
            }
            auto step = mover->updateMover(grid.getRhomusCentersPoints());

            if (step == MovementStep::CHECKPOINT_REACHED)
            {
                checkIfRemainignPathValid(*mover);
            }

            if (step == MovementStep::END_REACHED)
            {
                LOG_INFO("Mover ID ", mover->getId(), " reached its destination.");
                setDestination(*mover);
            }
        }
    }

private:
    void setDestination(MovableComponent &mover, bool shouldRecalculatePath = false)
    {
        const std::optional<uint32_t> startIndexOpt = getStartIndex(mover);
        if (!startIndexOpt)
        {
            return;
        }
        auto &context = pathContexts[&mover];

        if (not hasDestinations(mover, context))
        {
            return;
        }

        adjustCurrentStep(context, shouldRecalculatePath);

        const uint32_t startIndex = *startIndexOpt;
        const uint32_t goalIndex = getNextGoalIndex(context);

        const auto path = grid::aStarFindPath(startIndex, goalIndex, grid);
        mover.setPath(path);

        LOG_INFO(
            "Mover ID: ", mover.getId(),
            " new path set from startIndex ", startIndex,
            " to goalIndex ", goalIndex);
    }

    std::optional<uint32_t> getStartIndex(const MovableComponent &mover)
    {
        const auto position = mover.getPosition();
        const auto indexOpt = grid.getRhombiIndexByPosition(position);

        if (!indexOpt)
        {
            LOG_WARN(
                "Mover ID: ", mover.getId(),
                " is not on a valid rhombus. Cannot set destination.");
        }

        return indexOpt;
    }

    bool hasDestinations(const MovableComponent &mover, const PathContext &context)
    {
        if (!context.destinationIndices.empty())
            return true;

        LOG_WARN("No destination indices set for mover ID: ", mover.getId());
        return false;
    }

    void adjustCurrentStep(PathContext &context, bool shouldRecalculatePath)
    {
        if (!shouldRecalculatePath)
            return;

        const auto size = context.destinationIndices.size();
        context.currentStep = (context.currentStep == 0)
                                  ? size - 1
                                  : context.currentStep - 1;
    }

    uint32_t getNextGoalIndex(PathContext &context)
    {
        const auto &destinations = context.destinationIndices;

        uint32_t goalIndex = destinations[context.currentStep];
        goalIndex = calculateValidGoalindex(goalIndex);

        context.currentStep = (context.currentStep + 1) % destinations.size();
        return goalIndex;
    }

    uint32_t calculateValidGoalindex(uint32_t goalIndex)
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

    void checkIfRemainignPathValid(MovableComponent &mover)
    {
        const auto &remainingPath = mover.getRemainingPath();
        const auto &occupiedRhomus = grid.getModel().occupiedRhomus;

        for (size_t i = 1; i < remainingPath.size(); ++i)
        {
            if (std::find(occupiedRhomus.begin(), occupiedRhomus.end(), remainingPath[i]) != occupiedRhomus.end())
            {
                LOG_INFO("Remaining path for mover ID ", mover.getId(), " is blocked. Recalculating path.");
                setDestination(mover, true);
                break;
            }
        }
    }

    const grid::Grid &grid;
    std::vector<MovableComponent *> movers;
    std::unordered_map<MovableComponent *, PathContext> pathContexts;
};
