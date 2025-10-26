#pragma once
#include <SFML/System.hpp>

class FpsCounter
{
public:
    FpsCounter() = default;

    void update()
    {
        frameCount++;
        auto currentTime = clock.getElapsedTime();
        if (currentTime.asSeconds() - lastTime.asSeconds() >= 1.0f)
        {
            fps = frameCount;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    int getFps() const { return fps; }

private:
    sf::Clock clock;
    sf::Time lastTime{};
    int frameCount = 0;
    int fps = 0;
};