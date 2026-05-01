#pragma once

#include "MovableComponent.hpp"
#include "Bullet.hpp"
#include "event/Channel.hpp"
#include "grid/Grid.hpp"

class ShootingSystem
{
public:
    ShootingSystem(const grid::Grid &grid, event::Channel<ShootEvent> &shootChannel) : grid(grid), shootChannel(shootChannel) {}

    void addShooter(MovableComponent &shooter)
    {
        shootContexts.push_back(ShootContext{&shooter});
        LOG_INFO("Shooter ID: ", shooter.getId(), " added to ShootingSystem.");
    }
    void update()
    {
        for (auto &shooterCtx : shootContexts)
        {
            for (auto it = shooterCtx.bullets.begin();
                 it != shooterCtx.bullets.end();)
            {
                BulletContext &bulletCtx = *it;

                float dt = bulletCtx.flightClock.restart().asSeconds();
                bulletCtx.elapsedTimeMs += dt * 1000.f;

                float t =
                    bulletCtx.elapsedTimeMs /
                    static_cast<float>(bulletCtx.flightDurationMs);

                if (t >= 1.f)
                {
                    bulletCtx.bullet.setPosition(
                        bulletCtx.parabolicPath.back());
                    bulletCtx.bullet.setActive(false);

                    shootChannel.publish(ShootEvent{shooterCtx.shooter->getId()});

                    it = shooterCtx.bullets.erase(it); // remove finished bullet
                    continue;
                }

                float pathPos =
                    t * (bulletCtx.parabolicPath.size() - 1);

                std::size_t index = static_cast<std::size_t>(pathPos);
                float localT = pathPos - index;

                const sf::Vector2f &p0 =
                    bulletCtx.parabolicPath[index];
                const sf::Vector2f &p1 =
                    bulletCtx.parabolicPath[index + 1];

                bulletCtx.bullet.setPosition(
                    p0 + (p1 - p0) * localT);

                ++it;
            }
        }
    }

    void draw(sf::RenderTarget &target) const
    {
        for (const auto &shooterCtx : shootContexts)
        {
            for (const auto &bulletCtx : shooterCtx.bullets)
            {
                bulletCtx.bullet.draw(target);
            }
        }
    }

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window)
    {
        if (!event.is<sf::Event::KeyPressed>())
            return;

        const auto &key = *event.getIf<sf::Event::KeyPressed>();
        if (key.code != sf::Keyboard::Key::S)
            return;

        for (auto &shooterCtx : shootContexts)
        {
            auto shooterIndex =
                grid.getRhombiIndexByPosition(shooterCtx.shooter->getPosition());

            if (!shooterIndex)
            {
                LOG_WARN("Shooter ID ", shooterCtx.shooter->getId(),
                         " has invalid position.");
                continue;
            }

            BulletContext bulletCtx{Bullet(shooterCtx.shooter->getPosition())};
            bulletCtx.bullet.setActive(true);

            generateParabolicPath(
                bulletCtx,
                shooterIndex.value(),
                shooterCtx.targetIndex);

            shooterCtx.bullets.push_back(std::move(bulletCtx));

            LOG_INFO("Shooter ID ", shooterCtx.shooter->getId(),
                     " fired a bullet.");
        }
    }

private:
    struct BulletContext
    {
        Bullet bullet;
        std::vector<sf::Vector2f> parabolicPath;

        float elapsedTimeMs = 0.f;
        uint32_t flightDurationMs = 0;

        sf::Clock flightClock;
    };

    struct ShootContext
    {
        MovableComponent *shooter = nullptr;
        uint32_t targetIndex = 2000;

        std::vector<BulletContext> bullets;
    };

    void generateParabolicPath(
        BulletContext &bulletCtx,
        uint32_t startIndex,
        uint32_t targetIndex)
    {
        sf::Vector2f start = grid.getRhomusCenters()[startIndex];
        sf::Vector2f end = grid.getRhomusCenters()[targetIndex];

        constexpr float gravity = -900.f;
        constexpr float flightTime = 1.0f;
        constexpr int steps = 100;

        sf::Vector2f velocity;
        velocity.x = (end.x - start.x) / flightTime;
        velocity.y = (end.y - start.y +
                      0.5f * gravity * flightTime * flightTime) /
                     flightTime;

        bulletCtx.parabolicPath.clear();

        for (int i = 0; i <= steps; ++i)
        {
            float t = flightTime * i / steps;
            bulletCtx.parabolicPath.emplace_back(
                start.x + velocity.x * t,
                start.y + velocity.y * t - 0.5f * gravity * t * t);
        }

        bulletCtx.flightDurationMs = static_cast<uint32_t>(
            calculatePathLength(bulletCtx.parabolicPath) * 4.f);
    }

    float calculatePathLength(auto &parabolicPath)
    {
        float totalLength = 0.f;
        for (size_t i = 1; i < parabolicPath.size(); ++i)
        {
            sf::Vector2f diff = parabolicPath[i] - parabolicPath[i - 1];
            totalLength += std::sqrt(diff.x * diff.x + diff.y * diff.y);
        }
        LOG_INFO("Total length of parabolic path: ", totalLength);
        return totalLength;
    }

    const grid::Grid &grid;
    event::Channel<ShootEvent> &shootChannel;

    std::vector<ShootContext> shootContexts;
};
