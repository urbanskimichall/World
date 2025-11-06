#pragma once

#include <map>
#include <vector>
#include <cstdint>

namespace grid
{
    struct PathManager
    {
        std::map<std::pair<uint32_t, uint32_t>, std::vector<uint32_t>> paths;
    };
}