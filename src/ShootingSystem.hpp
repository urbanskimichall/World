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
            context.bullet.update();
            if (context.parabolicPath.empty() or context.currentStep >= context.parabolicPath.size())
            {
                continue;
            }
            float dt = context.flightClock.restart().asMilliseconds();
            context.elapsedTime += dt;
            if(context.elapsedTime >= 50.f) // advance every 50 ms
            {
                context.currentStep += 1;
                context.elapsedTime = 0.f;
            }
            if(context.currentStep >= context.parabolicPath.size())
            {
                LOG_INFO("ShootingSystem: Shooter ID: ", context.shooter->getId(), " bullet has reached the end of its path.");
                context.bullet.setActive(false);
                context.parabolicPath.clear();
                context.currentStep = 0;
            }
        }
    }

    void draw(sf::RenderTarget &target) const
    {
        for (const auto &context : shootContexts)
        {
            context.bullet.draw(target);
            if (context.parabolicPath.empty() or context.currentStep >= context.parabolicPath.size())
            {
                continue;
            }
            const auto &point = context.parabolicPath[context.currentStep];
            sf::CircleShape pathPoint(5.f);
            pathPoint.setFillColor(sf::Color::Red);
            pathPoint.setPosition(point);
            target.draw(pathPoint);
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
        uint32_t flightDurationMs = 10000;
        mutable sf::Clock flightClock;
        uint32_t currentStep = 0;
        float elapsedTime = 0.f;
        std::vector<sf::Vector2f> parabolicPath;
    };

    void generateParabolicPathFromStartIndexToTargetIndexOnTheGrid(ShootContext &context, uint32_t startIndex, uint32_t targetIndex)
    {
        if(context.parabolicPath.empty() == false)
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
    }

    const grid::Grid &grid;

    std::vector<ShootContext> shootContexts;
};
