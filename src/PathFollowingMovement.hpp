#pragma once

#include <unordered_map>
#include <vector>
#include "MovementStrategy.hpp"
#include "MovableComponent.hpp"

template <uint32_t formationId>
class PathFollowingMovement : public MovementStrategy
{
public:
    PathFollowingMovement(const grid::Grid &grid) : grid(grid) {}

    void addMover(MovableComponent &mover) override
    {
        LOG_INFO("Adding mover ID: ", mover.getId(), " to PathFollowingMovement with formation ID: ", formationId);
        initPosistionIndex[&mover] = mover.getCurrentCellIndex();
        setDestination(mover, 1250); // to do: calculate destination based on formation pattern and current position
    }

    uint16_t update(MovableComponent &mover) override
    {
        auto step = mover.updateMover(grid.getRhomusCentersPoints());

        if (step == MovementStep::END_REACHED)
        {
            LOG_INFO("Mover ID ", mover.getId(), " FORMATION ", formationId, " reached its destination.");
            // For now, just set the same destination again to keep it moving
            setDestination(mover, mover.getCurrentCellIndex() + 12110);
        }
        return 1;
    }

private:
    void setDestination(MovableComponent &mover, const uint32_t destinationIndex)
    {
        const auto startIndexOpt = grid.getRhombiIndexByPosition(mover.getPosition());
        if (!startIndexOpt)
        {
            LOG_WARN(
                "Mover ID: ", mover.getId(),
                " FORMATION ", formationId, " is not on a valid rhombus. Cannot set destination.");
            return;
        }
        const uint32_t startIndex = *startIndexOpt;

        const auto path = grid::aStarFindPath(startIndex, destinationIndex, grid);
        mover.setPath(path);

        LOG_INFO(
            "Mover ID: ", mover.getId(),
            " FORMATION ", formationId, " new path set from startIndex ", startIndex,
            " to destinationIndex ", destinationIndex);
    }

    std::unordered_map<MovableComponent *, std::vector<uint32_t>> moverPaths;
    std::unordered_map<MovableComponent *, uint32_t> initPosistionIndex;
    const grid::Grid &grid;
};