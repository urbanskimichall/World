#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include "components/BoxInfo.hpp"

class Tooltip
{
public:
    Tooltip() : text(font)
    {
        if (!font.openFromFile("assets/fonts/OpenSans-Italic.ttf"))
        {
            throw std::runtime_error("Failed to load font");
        }
        background.setFillColor(sf::Color(30, 30, 30, 220));
        background.setOutlineColor(sf::Color::White);
        background.setOutlineThickness(1.f);
        text.setFont(font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::White);
    }

    void show(const components::BoxInfo &info, sf::Vector2f position)
    {
        visible = true;

        text.setString(components::toString(info));
        text.setPosition(position + sf::Vector2f(10.f, 10.f));

        auto bounds = text.getGlobalBounds();
        background.setSize({bounds.size.x + 10.f, bounds.size.y + 10.f});
        background.setPosition(text.getPosition() - sf::Vector2f(5.f, 5.f));
    }

    void hide()
    {
        visible = false;
    }

    void draw(sf::RenderWindow &window) const
    {
        if (!visible)
        {
            return;
        }
        window.draw(background);
        window.draw(text);
    }

private:
    sf::Font font;
    bool visible = false;
    sf::RectangleShape background;
    sf::Text text;
};