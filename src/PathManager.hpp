#pragma once

#include <map>
#include <vector>
#include <cstdint>

struct PathContext
{
    uint32_t startIndex;
    uint32_t goalIndex;
    uint16_t moverId;
};

struct PathManager
{
                    // key: (startIndex, goalIndex, moverId), value: path as vector of node indices
    std::map<PathContext, std::vector<uint32_t>> paths;
};
