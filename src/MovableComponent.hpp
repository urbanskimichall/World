#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include "utils/Point.hpp"
#include "components/Component.hpp"
#include "Logger.hpp"
#include "ProgressBar.hpp"

enum class MovementStep : uint8_t
{
    ONGOING = 0,
    WAITING,
    CHECKPOINT_REACHED,
    END_REACHED,
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

        if (movementState == MovementStep::WAITING)
        {
            waitElapsed += dt;

            progressBar.setProgress(waitElapsed / 1.f);
            progressBar.setPosition(mover.getPosition() + sf::Vector2f(-20.f, -15.f));

            if (waitElapsed >= 1)
            {
                waitElapsed = 0.f;
                movementState = MovementStep::END_REACHED;
            }

            return movementState;
        }

        if (not hasActivePath() || centers.size() == 0)
        {
            //LOG_WARN("!!! Mover::updateMover Invalid path for mover ID: ", id, " currentSegment ", currentSegment, " hasActivePath() ",
            //         hasActivePath(), " centers.size() ", centers.size(), " !!!");
            movementState = MovementStep::INVALID_PATH;
            return movementState;
        }

        if (currentSegment >= path.size() - 1)
        {
            movementState = MovementStep::WAITING;
            return movementState;
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
            return MovementStep::CHECKPOINT_REACHED;
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
            mover.setPosition({static_cast<float>(p1.x), static_cast<float>(p1.y)});
            movementState = MovementStep::CHECKPOINT_REACHED;
            return movementState;
        }

        // Interpolate position along current segment
        const float x = p0.x + (p1.x - p0.x) * segmentProgress;
        const float y = p0.y + (p1.y - p0.y) * segmentProgress;
        mover.setPosition({x, y});
        movementState = MovementStep::ONGOING;
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
        if(movementState == MovementStep::WAITING)
        {
            progressBar.draw(target);
        }
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

    uint32_t getCurrentCellIndex() const
    {
        sf::Vector2f pos = getPosition();
        return grid.getRhombiIndexByPosition(pos).value_or(UINT32_MAX);
    }

protected:
    std::vector<uint32_t> path;
    uint32_t currentSegment = 0; // which segment (path[i] -> path[i+1]) we are on
    float segmentProgress = 0.f; // 0..1 interpolation along the segment
    float moveSpeed = 100.f;     // pixels per second
    sf::Clock clock;
    sf::CircleShape mover{4.f};
    MovementStep movementState = MovementStep::INVALID_PATH;
    float waitElapsed = 0.f;
    ProgressBar progressBar{40.f, 6.f};
};