#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>

class TextureManager
{
public:
    // Get the shared texture instance
    static TextureManager& instance()
    {
        static TextureManager tm;
        return tm;
    }

    // Loads a texture once and returns a reference to it
    const sf::Texture& get(const std::string& filename)
    {
        auto it = textures.find(filename);
        if (it != textures.end())
        {
            return *it->second;
        }

        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(filename))
        {
            throw std::runtime_error("Failed to load texture: " + filename);
        }

        // Store and return
        const sf::Texture& ref = *texture;
        textures.emplace(filename, std::move(texture));
        return ref;
    }

private:
    TextureManager() = default;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
};