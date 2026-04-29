#pragma once

#include <vector>
#include <cstdint>

class MovableComponent;

class MovementStrategy
{
public:
    virtual ~MovementStrategy() = default;

    virtual void addMover(MovableComponent& mover) = 0;
    virtual uint16_t update(MovableComponent &mover) = 0;
};