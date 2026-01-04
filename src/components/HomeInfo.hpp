#pragma once

#include <string>

namespace components
{
    struct HomeInfo
    {
        std::string name;
        int level;
        int capacity;
        int occupants;
        int happiness; // %
        int farmers;
        int builders;
        int soldiers;
    };
    std::string toString(const HomeInfo &info);
}