#pragma once

#include <vector>
#include <memory>
#include <functional>

class MovableComponent;

struct MoverReachedIndexEvent
{
    MovableComponent *mover;
    uint32_t index;
    uint32_t step;
};

class EventBus
{
public:
    using Callback = std::function<void(const MoverReachedIndexEvent&)>;

    void subscribe(Callback cb)
    {
        listeners.push_back(cb);
    }

    void publish(const MoverReachedIndexEvent& e)
    {
        for(auto& cb : listeners)
        {
            cb(e);
        }
    }

private:
    std::vector<Callback> listeners;
};