#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "resource/FontManager.hpp"

class ProgressBar
{
public:
    ProgressBar(float width, float height)
        : background({width, height}),
          foreground({0.f, height}),
          font(&FontManager::instance().get("assets/fonts/OpenSans-Italic.ttf")),
          text(*font)
    {
        background.setFillColor(sf::Color(50, 50, 50, 180));
        background.setOutlineThickness(1.f);
        background.setOutlineColor(sf::Color::Black);

        foreground.setFillColor(sf::Color::Green);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::White);
    }

    void setProgress(float progress)
    {
        progress = std::clamp(progress, 0.f, 1.f);
        foreground.setSize({background.getSize().x * progress,
                            background.getSize().y});
        text.setString(std::to_string(static_cast<int>(progress * 100)) + "%");
    }

    void setPosition(const sf::Vector2f &pos)
    {
        background.setPosition(pos);
        foreground.setPosition(pos);
        text.setPosition(pos + sf::Vector2f(5.f, 0.f));
    }

    void draw(sf::RenderTarget &target) const
    {
        target.draw(background);
        target.draw(foreground);
        target.draw(text);
    }

private:
    sf::RectangleShape background;
    sf::RectangleShape foreground;
    const sf::Font* font;
    sf::Text text;
};