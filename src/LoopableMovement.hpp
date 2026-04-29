#pragma once

#include <unordered_map>
#include <vector>
#include "grid/Grid.hpp"
#include "MovableComponent.hpp"
#include "grid/Pathfinder.hpp"
#include "MovementStrategy.hpp"

class LoopableMovement : public MovementStrategy
{
public:
    LoopableMovement(const grid::Grid &grid, const std::vector<uint32_t> &destinationIndices) : grid(grid), destinationIndices(destinationIndices) {}

    void addMover(MovableComponent &mover) override;

    uint16_t update(MovableComponent &mover) override;

private:
    struct PathContext
    {
        uint32_t currentStep;
        std::vector<uint32_t> destinationIndices;
    };

    void setDestination(MovableComponent &mover, bool shouldRecalculatePath = false);

    std::optional<uint32_t> getStartIndex(const MovableComponent &mover);

    bool hasDestinations(const MovableComponent &mover, const PathContext &context);

    void adjustCurrentStep(PathContext &context, bool shouldRecalculatePath);

    uint32_t getNextGoalIndex(PathContext &context);

    uint32_t calculateValidGoalindex(uint32_t goalIndex);

    void checkIfRemainignPathValid(MovableComponent &mover);

    const grid::Grid &grid;
    // std::vector<MovableComponent *> movers;
    std::unordered_map<MovableComponent *, PathContext> pathContexts;
    const std::vector<uint32_t> destinationIndices;
};
