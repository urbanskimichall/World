#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include "MovableComponent.hpp"
#include "MovementStrategy.hpp"

class MovementSystem
{
public:
    void addMover(
        MovableComponent& mover,
        std::unique_ptr<MovementStrategy> strategy)
    {
        strategies[&mover] = std::move(strategy);
        strategies[&mover]->addMover(mover); // Initialize with current position
        movers.push_back(&mover);
        LOG_INFO("Mover ID: ", mover.getId(), " added to MovementSystem ", movers.size()," total movers.");
    }

    void update()
    {
        for (auto* mover : movers)
        {
            strategies[mover]->update(*mover);
        }
    }

private:
    std::vector<MovableComponent*> movers;
    std::unordered_map<MovableComponent*, std::unique_ptr<MovementStrategy>> strategies;
};