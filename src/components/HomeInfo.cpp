#include "HomeInfo.hpp"

namespace components
{
    std::string toString(const HomeInfo &info)
    {
        return "Name: " + info.name + "\n" +
               "Level: " + std::to_string(info.level) + "\n" +
               "Capacity: " + std::to_string(info.capacity) + "\n" +
               "Occupants: " + std::to_string(info.occupants) + "\n" +
               "Happiness: " + std::to_string(info.happiness) + "%\n" +
               "Farmers: " + std::to_string(info.farmers) + "\n" +
               "Builders: " + std::to_string(info.builders) + "\n" +
               "Soldiers: " + std::to_string(info.soldiers) + "\n";
    }
} // namespace components