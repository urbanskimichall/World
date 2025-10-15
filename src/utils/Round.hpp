#pragma once
#include <cmath>

namespace utils
{
    template <typename T>
    T roundUpToMultiple(T value, T multiple)
    {
        if (multiple == static_cast<T>(0))
        {
            return value;
        }
        return std::ceil(value / multiple) * multiple;
    }
} // namespace utils