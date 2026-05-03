#pragma once

#include <vector>
#include <memory>
#include <functional>

struct MovementStepEvent
{
    uint16_t entityId;
    uint16_t step;
    uint32_t cellIndex;
};

struct ShootEvent
{
    uint16_t entityId;
};

struct FoodDeliveredEvent
{
    uint16_t entityId;
    uint32_t cellIndex;
    uint32_t homeIndex;
    uint16_t amount;
};

namespace event
{

template<typename Event>
class Channel
{
public:
    using Handler = std::function<void(const Event&)>;

    void subscribe(Handler h)
    {
        handlers.push_back(std::move(h));
    }

    void publish(const Event& e)
    {
        for (auto& h : handlers)
            h(e);
    }

private:
    std::vector<Handler> handlers;
};

} // namespace event


// class EventDispatcher
// {
// public:
//     template<typename T>
//     void subscribe(std::function<void(const T&)> handler)
//     {
//         auto wrapper = [handler](const Event& e)
//         {
//             if (const auto* ev = std::get_if<T>(&e))
//             {
//                 handler(*ev);
//             }
//         };
//         handlers.push_back(std::move(wrapper));
//     }

//     void publish(const Event& event)
//     {
//         for (auto& h : handlers)
//             h(event);
//     }

// private:
//     std::vector<std::function<void(const Event&)>> handlers;
// };
