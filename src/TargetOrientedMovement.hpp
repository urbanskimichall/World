#pragma once

#include "MovementStrategy.hpp"
#include "MovableComponent.hpp"
#include "Logger.hpp"

class TargetOrientedMovement : public MovementStrategy
{
public:
    TargetOrientedMovement(const grid::Grid &grid) : grid(grid) {}

    void addMover(MovableComponent &mover, const std::vector<uint32_t> &destinationIndices) override
    {
        LOG_INFO("Adding target-oriented mover ID: ", mover.getId(), " to MovementSystem.");
        moverToHighlightedIndex[&mover] = UINT32_MAX;
        mover.setPosition(grid.getClosestRhombusCenterPositionToCurrentPosition(mover.getPosition())); // Ensure mover is at its current position
        setDestination(mover);
    }

    void update(MovableComponent &mover) override
    {
        auto step = mover.updateMover(grid.getRhomusCentersPoints());
        const std::optional<uint32_t> highlightedIndexOpt = grid.getHighlightedIndexIfNotOccupied();
        if (!highlightedIndexOpt)
        {
            LOG_WARN("Target oriented: No valid highlighted rhombus under mouse for mover ID: ", mover.getId());
            return;
        }
        if (highlightedIndexOpt.value() == UINT32_MAX)
        {
            LOG_WARN("Target oriented: Highlighted rhombus under mouse is invalid for mover ID: ", mover.getId());
            return;
        }
        const uint32_t highlightedIndex = highlightedIndexOpt.value();
        uint32_t& currentHighlightedIndex = moverToHighlightedIndex[&mover];
        if (currentHighlightedIndex != highlightedIndex)
        {
            if (step == MovementStep::INVALID_PATH or step == MovementStep::END_REACHED or step == MovementStep::CHECKPOINT_REACHED)
            {
                LOG_INFO("Target oriented: Recalculating path for mover ID: ", mover.getId(),
                         " due to invalid path and new highlighted rhombus ", highlightedIndex);
                setDestination(mover);
                currentHighlightedIndex = highlightedIndex;
            }
        }
    }

private:
    void setDestination(MovableComponent &mover)
    {
        const uint32_t currentHighlightedIndex = moverToHighlightedIndex[&mover];
        if (currentHighlightedIndex == UINT32_MAX)
        {
            LOG_WARN("Target oriented: No rhombus highlighted under mouse for mover ID: ", mover.getId());
            return;
        }
        const std::optional<uint32_t> startIndexOpt = getStartIndex(mover);
        if (!startIndexOpt)
        {
            return;
        }

        const uint32_t startIndex = *startIndexOpt;
        const uint32_t goalIndex = currentHighlightedIndex;

        const auto path = grid::aStarFindPath(startIndex, goalIndex, grid);
        if (path.empty())
        {
            LOG_WARN("No path found for target oriented mover ID: ", mover.getId(), " from rhombus ", startIndex, " to ", goalIndex);
            return;
        }
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

    const grid::Grid &grid;
    std::unordered_map<MovableComponent *, uint32_t> moverToHighlightedIndex;
};