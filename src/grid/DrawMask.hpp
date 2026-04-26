#pragma once

#include <bitset>

namespace grid
{
    enum class DrawElement : size_t
    {
        NodePoints = 0,
        Rhombi,
        HighlightedRhombi,
        SelectedRhombi,
        OccupiedDots,
        BoundaryRhombi,
        Count
    };

    using DrawMask = std::bitset<static_cast<size_t>(DrawElement::Count)>;
}