#pragma once

#include <vector>
#include <cstdint>

class MovableComponent;

class MovementStrategy
{
public:
    virtual ~MovementStrategy() = default;

    virtual void addMover(MovableComponent& mover, const std::vector<uint32_t> &destinationIndices) = 0;
    virtual void update(MovableComponent &mover) = 0;
};