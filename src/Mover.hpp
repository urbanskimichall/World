#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include "utils/Point.hpp"
#include "components/Component.hpp"

enum class MovementStep : uint8_t
{
    ONGOING = 0,
    CHECKPOINT_REACHED = 1,
    END_REACHED = 2,
    INVALID_PATH = 0xFF
};

class MovableComponent : public components::Component
{
public:
    MovableComponent(grid::Grid &grid, uint16_t id) : components::Component(grid, id) {}
    void init()
    {
        mover.setFillColor(sf::Color::White);
    }

    components::Capability capabilities() const override
    {
        return components::Capability::Draggable | components::Capability::Movable;
    }

    void setPath(std::vector<uint32_t> newPath)
    {
        path = std::move(newPath);
        currentSegment = 0;
        segmentProgress = 0.f;
    }

    bool hasActivePath() const
    {
        return path.size() > 1;
    }

    MovementStep updateMover(const std::vector<utils::Point> &centers)
    {
        float dt = clock.restart().asSeconds();
        if (not hasActivePath() || centers.size() == 0)
        {
            LOG_WARN("!!! Mover::updateMover Invalid path for mover ID: ", id, " currentSegment ", currentSegment, " hasActivePath() ",
                hasActivePath(), " centers.size() ", centers.size(), " !!!");
            return MovementStep::INVALID_PATH;
        }
        if(currentSegment >= path.size() - 1)
        {
            return MovementStep::END_REACHED;
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
            return MovementStep::ONGOING;
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
            return MovementStep::CHECKPOINT_REACHED;
        }
        else
        {
            // Interpolate position along current segment
            const float x = p0.x + (p1.x - p0.x) * segmentProgress;
            const float y = p0.y + (p1.y - p0.y) * segmentProgress;
            mover.setPosition({x, y});
        }
        return MovementStep::ONGOING;
    }

    std::vector<uint32_t> getRemainingPath() const
    {
        if (currentSegment >= path.size())
        {
            return {};
        }   
        return std::vector<uint32_t>(path.begin() + currentSegment, path.end());
    }

    void draw(sf::RenderTarget &target) const override
    {
        target.draw(mover);
    }
    void setPosition(const sf::Vector2f &pos) override
    {
        mover.setPosition(pos);
    }
    sf::Vector2f getPosition() const override
    {
        return mover.getPosition();
    }
    std::vector<sf::Vector2f> getTransformedPoints() const override
    {
        sf::FloatRect bounds = mover.getGlobalBounds();
        std::vector<sf::Vector2f> points(4);
        points[0] = {bounds.position};
        points[1] = {bounds.position.x + bounds.size.x, bounds.position.y};
        points[2] = {bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y};
        points[3] = {bounds.position.x, bounds.position.y + bounds.size.y};
        return points;
    }
    components::BoxInfo getInfo() const override
    {
        components::BoxInfo info;
        info.name = "Mover";
        info.level = 0;
        info.capacity = 0;
        info.occupants = 0;
        info.happiness = 0;
        info.farmers = 0;
        info.builders = 0;
        info.soldiers = 0;
        return info;
    }
    sf::Vector2f getSize() const override
    {
        sf::FloatRect bounds = mover.getGlobalBounds();
        return {bounds.size.x, bounds.size.y};
    }

protected:
    std::vector<uint32_t> path;
    uint32_t currentSegment = 0; // which segment (path[i] -> path[i+1]) we are on
    float segmentProgress = 0.f; // 0..1 interpolation along the segment
    float moveSpeed = 100.f;     // pixels per second
    sf::Clock clock;
    sf::CircleShape mover{4.f};
};