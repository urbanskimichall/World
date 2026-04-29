#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include "MovableComponent.hpp"
#include "MovementStrategy.hpp"
#include "EventBus.hpp"

class MovementSystem
{
public:
    MovementSystem(EventBus &eventBus) : eventBus(eventBus) {}

    void addMover(
        MovableComponent &mover,
        std::unique_ptr<MovementStrategy> strategy)
    {
        strategies[&mover] = std::move(strategy);
        strategies[&mover]->addMover(mover); // Initialize with current position
        movers.push_back(&mover);
        LOG_INFO("Mover ID: ", mover.getId(), " added to MovementSystem ", movers.size(), " total movers.");
    }

    void update()
    {
        for (auto *mover : movers)
        {
            uint16_t step = strategies[mover]->update(*mover);

            eventBus.publish(
                MoverReachedIndexEvent{
                    mover,
                    mover->getCurrentCellIndex(),
                    step});
            // publish each event and let subscribers decide if they want to react to it based on mover ID or index or step
        }
    }

private:
    std::vector<MovableComponent *> movers;
    std::unordered_map<MovableComponent *, std::unique_ptr<MovementStrategy>> strategies;
    EventBus &eventBus;
};