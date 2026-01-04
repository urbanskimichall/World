#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include "utils/Point.hpp"

class Mover
{
public:
    void init()
    {
        mover.setFillColor(sf::Color::White);
    }

    void updateMover(const std::vector<uint32_t> &path,
                     const std::vector<utils::Point> &centers)
    {
        float dt = clock.restart().asSeconds();
        if (path.size() < 2 || currentSegment >= path.size() - 1)
        {
            return; // no movement or reached end
        }

        // Get segment endpoints
        const utils::Point p0 = centers[path[currentSegment]];
        const utils::Point p1 = centers[path[currentSegment + 1]];

        // Compute segment length
        const float dist = static_cast<float>(p0.distanceTo(p1));
        if (dist < 0.0001f)
        {
            currentSegment++;
            segmentProgress = 0.f;
            return;
        }

        // Increase progress based on time and speed
        // independently on distance between next point on path
        segmentProgress += (moveSpeed * dt) / dist;

        if (segmentProgress >= 1.f)
        {
            // Move to next segment
            segmentProgress = 0.f;
            currentSegment++;

            // Snap to end point for visual accuracy
            mover.setPosition({p1.x, p1.y});
        }
        else
        {
            // Interpolate position along current segment
            const float x = p0.x + (p1.x - p0.x) * segmentProgress;
            const float y = p0.y + (p1.y - p0.y) * segmentProgress;
            mover.setPosition({x, y});
        }
    }
    void draw(sf::RenderWindow &target) const
    {
        target.draw(mover);
    }

protected:
    uint32_t currentSegment = 0; // which segment (path[i] -> path[i+1]) we are on
    float segmentProgress = 0.f; // 0..1 interpolation along the segment
    float moveSpeed = 100.f;     // pixels per second
    sf::Clock clock;
    sf::CircleShape mover{4.f};
};