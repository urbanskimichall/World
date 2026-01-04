#pragma once

#include <map>
#include <vector>
#include <cstdint>

struct PathManager
{
                    // key: (startIndex, goalIndex), value: path as vector of node indices
    std::map<std::pair<uint32_t, uint32_t>, std::vector<uint32_t>> paths;
};
