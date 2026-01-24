#pragma once

#include "MovableComponent.hpp"
#include "Bullet.hpp"
#include "grid/Grid.hpp"

class ShootingSystem
{
public:
    ShootingSystem(const grid::Grid &grid) : grid(grid) {}

    void addShooter(MovableComponent &shooter)
    {
        shootContexts.push_back(ShootContext{&shooter, Bullet(shooter.getPosition())});
        LOG_INFO("Shooter ID: ", shooter.getId(), " added to ShootingSystem.");
    }
    void update()
    {
        for (auto &context : shootContexts)
        {
            float dt = context.flightClock.restart().asSeconds();
            if (context.parabolicPath.size() < 2)
                continue;

            context.elapsedTime += dt * 1000.f; // ms

            float normalizedTime =
                context.elapsedTime / static_cast<float>(context.flightDurationMs);

            if (normalizedTime >= 1.f)
            {
                // Snap to final point
                context.bullet.setPosition(context.parabolicPath.back());
                context.bullet.setActive(false);

                context.parabolicPath.clear();
                context.currentStep = 0;
                context.elapsedTime = 0.f;

                LOG_INFO("Bullet reached target.");
                continue;
            }

            // Map time → path
            float pathPos =
                normalizedTime * (context.parabolicPath.size() - 1);

            std::size_t index =
                static_cast<std::size_t>(pathPos);

            float localT = pathPos - index;

            const sf::Vector2f &p0 = context.parabolicPath[index];
            const sf::Vector2f &p1 = context.parabolicPath[index + 1];

            // Linear interpolation
            sf::Vector2f position =
                p0 + (p1 - p0) * localT;

            context.bullet.setPosition(position);
        }
    }

    void draw(sf::RenderTarget &target) const
    {
        for (const auto &context : shootContexts)
        {
            context.bullet.draw(target);
        }
    }

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window)
    {
        if (event.is<sf::Event::KeyPressed>())
        {
            const auto &m = *event.getIf<sf::Event::KeyPressed>();
            if (m.code == sf::Keyboard::Key::S)
            {
                for (auto &context : shootContexts)
                {
                    LOG_INFO("ShootingSystem: Shooter ID: ", context.shooter->getId(), " is attempting to shoot.");
                    auto shooterIndex = grid.getRhombiIndexByPosition(context.shooter->getPosition()); // Assuming ID corresponds to grid index
                    if (!shooterIndex.has_value())
                    {
                        LOG_WARN("ShootingSystem: Shooter ID: ", context.shooter->getId(), " has invalid position for shooting.");
                        continue;
                    }

                    generateParabolicPathFromStartIndexToTargetIndexOnTheGrid(context, shooterIndex.value(), context.targetIndex);
                    context.bullet.setActive(true);
                    LOG_INFO("Shooter ID: ", context.shooter->getId(), " fired at target index: ", context.targetIndex);
                }
            }
        }
    }

private:
    struct ShootContext
    {
        MovableComponent *shooter;
        Bullet bullet;
        uint32_t targetIndex = 2000;
        uint32_t flightDurationMs = 2000;
        mutable sf::Clock flightClock;
        uint32_t currentStep = 0;
        float elapsedTime = 0.f;
        std::vector<sf::Vector2f> parabolicPath;
        uint32_t totalPathLength = 0;
    };

    void generateParabolicPathFromStartIndexToTargetIndexOnTheGrid(ShootContext &context, uint32_t startIndex, uint32_t targetIndex)
    {
        if (context.parabolicPath.empty() == false)
        {
            LOG_INFO("ShootingSystem: Shooter ID: ", context.shooter->getId(), " already has an active parabolic path. Skipping generation.");
            return;
        }
        if (startIndex >= grid.getRhomusCenters().size() ||
            targetIndex >= grid.getRhomusCenters().size())
        {
            LOG_WARN("Invalid start or target index for parabolic path generation.");
            return;
        }

        sf::Vector2f start = grid.getRhomusCenters()[startIndex];
        sf::Vector2f end = grid.getRhomusCenters()[targetIndex];

        const float gravity = -900.f;
        const float flightTime = 1.0f;

        sf::Vector2f velocity;
        velocity.x = (end.x - start.x) / flightTime;
        velocity.y = (end.y - start.y + 0.5f * gravity * flightTime * flightTime) / flightTime;

        std::vector<sf::Vector2f> path;
        constexpr int steps = 100;

        for (int i = 0; i <= steps; ++i)
        {
            float t = flightTime * i / steps;
            float x = start.x + velocity.x * t;
            float y = start.y + velocity.y * t - 0.5f * gravity * t * t;
            path.emplace_back(x, y);
        }

        context.parabolicPath = path;
        context.currentStep = 0;
        context.elapsedTime = 0.f;
        calculateTotalLengthOfParabolicPath(context);
    }

    void calculateTotalLengthOfParabolicPath(ShootContext &context)
    {
        float totalLength = 0.f;
        for (size_t i = 1; i < context.parabolicPath.size(); ++i)
        {
            sf::Vector2f diff = context.parabolicPath[i] - context.parabolicPath[i - 1];
            totalLength += std::sqrt(diff.x * diff.x + diff.y * diff.y);
        }
        LOG_INFO("Total length of parabolic path: ", totalLength);
        context.flightDurationMs = static_cast<uint32_t>(totalLength * 4);
    }

    const grid::Grid &grid;

    std::vector<ShootContext> shootContexts;
};
