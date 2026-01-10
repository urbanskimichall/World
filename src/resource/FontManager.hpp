#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

class FontManager
{
public:
    static FontManager &instance()
    {
        static FontManager fm;
        return fm;
    }
    const sf::Font &get(const std::string &filename)
    {
        auto it = fonts.find(filename);
        if (it != fonts.end())
        {
            return *it->second;
        }

        auto font = std::make_unique<sf::Font>();
        if (!font->openFromFile(filename))
        {
            throw std::runtime_error("Failed to load font: " + filename);
        }

        const sf::Font &ref = *font;
        fonts.emplace(filename, std::move(font));
        return ref;
    }

private:
    FontManager() = default;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;
};