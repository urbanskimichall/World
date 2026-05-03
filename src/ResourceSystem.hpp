#pragma once

#include "event/Channel.hpp"

struct Storage
{
    uint32_t food = 0;
    uint32_t wood = 0;  // to be implemented
    uint32_t stone = 0; // to be implemented
};

using HomeIndex = uint32_t;

class ResourceSystem
{
public:
    ResourceSystem(event::Channel<FoodDeliveredEvent> &foodDeliveryChannel)
        : foodDeliveryChannel(foodDeliveryChannel)
    {
    }

    void update(const FoodDeliveredEvent &event)
    {
        storageOnHomes[event.homeIndex].food += event.amount;
    }

private:
    event::Channel<FoodDeliveredEvent> &foodDeliveryChannel;
    std::unordered_map<HomeIndex, Storage> storageOnHomes; // homeIndex -> total food delivered
};