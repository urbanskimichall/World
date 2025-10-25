#pragma once
#include <cstdint>

namespace grid
{
    constexpr float GRID_SPACING = 20.0f;
    constexpr float MIN_GRID_SPACING = 5.0f;
    constexpr float MAX_GRID_SPACING = 50.0f;

     // 2:1 rhombus dimensions
    constexpr float RHOMBUS_DIAG_X = 100.0f;
    constexpr float RHOMBUS_DIAG_Y = 25.f;

    constexpr std::uint32_t numOfRows = 50;
}