#pragma once

#include <string>

namespace components
{
    struct BoxInfo
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
    std::string toString(const BoxInfo &info);
}