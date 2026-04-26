#include "GridRenderer.hpp"
#include "../Logger.hpp"

namespace grid
{
    void GridRenderer::draw(const GridModel &model, sf::RenderWindow &window, const sf::FloatRect &bounds) const
    {
        sf::VertexArray points(sf::PrimitiveType::Points);
        for (const auto &node : model.nodes)
        {
            if (!bounds.contains({node.point.x, node.point.y}))
                continue;

            sf::Color color = node.isHighlighted ? sf::Color::Red : sf::Color::Blue;
            points.append(sf::Vertex({node.point.x, node.point.y}, color));
        }
        window.draw(points);
    }

    void GridRenderer::drawRhombi(const GridModel &model, sf::RenderWindow &window, const sf::FloatRect &bounds,
                                  uint32_t highlightedIndex, const std::vector<uint32_t> &selectedIndices) const
    {
        auto isVisible = [&](const Rhombus &r)
        {
            return bounds.contains({r.a.x, r.a.y}) ||
                   bounds.contains({r.b.x, r.b.y}) ||
                   bounds.contains({r.c.x, r.c.y}) ||
                   bounds.contains({r.d.x, r.d.y});
        };

        for (size_t i = 0; i < model.rhombi.size(); ++i)
        {
            const Rhombus &rh = model.rhombi[i];
            if (!isVisible(rh))
                continue;

            sf::ConvexShape shape;
            shape.setPointCount(4);
            shape.setPoint(0, {rh.a.x, rh.a.y});
            shape.setPoint(1, {rh.b.x, rh.b.y});
            shape.setPoint(2, {rh.c.x, rh.c.y});
            shape.setPoint(3, {rh.d.x, rh.d.y});

            if (i == highlightedIndex or model.nodes[i].isBoundary)
            {
                shape.setFillColor(sf::Color(255, 0, 0, 50));
                shape.setOutlineColor(sf::Color::Red);
                shape.setOutlineThickness(1.5f);
                window.draw(shape);
            }
            else if (std::find(selectedIndices.begin(), selectedIndices.end(), i) != selectedIndices.end())
            {
                shape.setFillColor(sf::Color(0, 255, 0, 100));
                shape.setOutlineColor(sf::Color::Green);
            }
            else
            {
                shape.setFillColor(sf::Color(0, 0, 255, 30));
                shape.setOutlineColor(sf::Color::Blue);
            }
            if (const auto underMouse{model.highlightedByMouseRhomusInd};
                underMouse.has_value() and i == underMouse.value())
            {
                shape.setFillColor(sf::Color(128, 128, 128, 30));
                shape.setOutlineColor(sf::Color::Yellow);
            }

            if (std::find(model.occupiedRhomus.begin(), model.occupiedRhomus.end(), i) != model.occupiedRhomus.end())
            {
                sf::CircleShape dot(3.f);
                dot.setFillColor(sf::Color::Red);
                dot.setPosition({model.rhombusCenters[i].x - 3.f, model.rhombusCenters[i].y - 3.f});
                window.draw(dot);
            }
            else
            {
                sf::CircleShape dot(3.f);
                dot.setFillColor(sf::Color::Yellow);
                dot.setPosition({model.rhombusCenters[i].x - 3.f, model.rhombusCenters[i].y - 3.f});
                //window.draw(dot);
            }
        }
    }
}