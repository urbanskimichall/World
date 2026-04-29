#pragma once

#include <cstdint>
#include <vector>

struct FarmerPath
{
    uint32_t homeIndex{0xFFFFFFFF};
    uint32_t fieldIndex{0xFFFFFFFF};
    uint32_t storageIndex{0xFFFFFFFF};
};

class FarmerPathFactory
{
public:
    static std::vector<uint32_t> getPathIndices(const FarmerPath &path)
    {
        return {path.homeIndex, path.fieldIndex, path.storageIndex};
    }
};
