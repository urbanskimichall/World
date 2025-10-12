#pragma once

#include <cmath>
#include <utility>

namespace utils
{
    struct Point
    {
        double x;
        double y;

        Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}

        double distanceTo(const Point &other) const
        {
            return std::hypot(other.x - x, other.y - y);
        }

        Point operator+(const Point &other) const
        {
            return Point(x + other.x, y + other.y);
        }

        Point operator-(const Point &other) const
        {
            return Point(x - other.x, y - other.y);
        }

        Point operator*(double scalar) const
        {
            return Point(x * scalar, y * scalar);
        }

        Point operator/(double scalar) const
        {
            return Point(x / scalar, y / scalar);
        }
        
        bool operator==(const Point &other) const
        {
            return std::abs(x - other.x) < 1e-9 && std::abs(y - other.y) < 1e-9;
        }
    };
}