#include "World.hpp"
#include <SFML/Graphics.hpp>

void World::create()
{
        sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 + C++23");
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);
        window.setFramerateLimit(60);

        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
                else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    {
                        window.close();
                    }
                }
            }

            window.clear();
            grid.draw(window);
            //window.draw(shape);
            window.display();
        }
}