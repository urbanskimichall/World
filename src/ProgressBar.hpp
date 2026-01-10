#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

class ProgressBar
{
public:
    ProgressBar(float width, float height)
        : background({width, height}),
          foreground({0.f, height})
    {
        background.setFillColor(sf::Color(50, 50, 50, 180));
        background.setOutlineThickness(1.f);
        background.setOutlineColor(sf::Color::Black);

        foreground.setFillColor(sf::Color::Green);
    }

    void setProgress(float progress)
    {
        progress = std::clamp(progress, 0.f, 1.f);
        foreground.setSize({background.getSize().x * progress,
                            background.getSize().y});
    }

    void setPosition(const sf::Vector2f& pos)
    {
        background.setPosition(pos);
        foreground.setPosition(pos);
    }

    void draw(sf::RenderTarget& target) const
    {
        target.draw(background);
        target.draw(foreground);
    }

private:
void printProgressPercentage(float progress)
{
    int percentage = static_cast<int>(progress * 100);
    std::cout << "Progress: " << percentage << "%\n";
}
    sf::RectangleShape background;
    sf::RectangleShape foreground;
};