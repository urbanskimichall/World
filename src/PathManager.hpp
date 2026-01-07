#pragma once

#include <map>
#include <vector>
#include <cstdint>

struct PathContext
{
    uint32_t startIndex;
    uint32_t goalIndex;
    std::vector<uint32_t> path;
};

struct PathManager
{
    std::map<uint16_t, PathContext> paths;
};
