#include "GridRenderer.hpp"
#include "../Logger.hpp"

namespace grid
{
    void GridRenderer::draw(const GridModel &model, sf::RenderWindow &window, const sf::FloatRect &bounds) const
    {
        if (!drawMask.test(static_cast<size_t>(DrawElement::NodePoints)))
            return;

        sf::VertexArray points(sf::PrimitiveType::Points);
        for (const auto &node : model.nodes)
        {
            if (!bounds.contains({static_cast<float>(node.point.x), static_cast<float>(node.point.y)}))
                continue;

            sf::Color color = node.isHighlighted ? sf::Color::Red : sf::Color::Blue;
            points.append(sf::Vertex({static_cast<float>(node.point.x), static_cast<float>(node.point.y)}, color));
        }
        window.draw(points);
    }

    void GridRenderer::drawRhombi(const GridModel &model, sf::RenderWindow &window, const sf::FloatRect &bounds,
                                  uint32_t highlightedIndex, const std::vector<uint32_t> &selectedIndices) const
    {
        auto isVisible = [&](const Rhombus &r)
        {
            return bounds.contains({static_cast<float>(r.a.x), static_cast<float>(r.a.y)}) ||
                   bounds.contains({static_cast<float>(r.b.x), static_cast<float>(r.b.y)}) ||
                   bounds.contains({static_cast<float>(r.c.x), static_cast<float>(r.c.y)}) ||
                   bounds.contains({static_cast<float>(r.d.x), static_cast<float>(r.d.y)});
        };

        const bool drawAllRhombi = drawMask.test(static_cast<size_t>(DrawElement::Rhombi));
        const bool drawHighlighted = drawMask.test(static_cast<size_t>(DrawElement::HighlightedRhombi));
        const bool drawSelected = drawMask.test(static_cast<size_t>(DrawElement::SelectedRhombi));
        const bool drawBoundary = drawMask.test(static_cast<size_t>(DrawElement::BoundaryRhombi));
        const bool drawDots = drawMask.test(static_cast<size_t>(DrawElement::OccupiedDots));

        for (size_t i = 0; i < model.rhombi.size(); ++i)
        {
            if (model.rhombi.size() != model.rhombusCenters.size())
            {
                LOG_ERROR("Rhombi size and centers size mismatch: ", model.rhombi.size(), " vs ", model.rhombusCenters.size());
                return;
            }

            const Rhombus &rh = model.rhombi[i];
            if (!isVisible(rh))
                continue;

            const bool highlighted = (i == highlightedIndex);
            const bool selected = std::find(selectedIndices.begin(), selectedIndices.end(), i) != selectedIndices.end();
            const bool boundary = (i < model.nodes.size()) && model.nodes[i].isBoundary;

            if (!drawAllRhombi && !((highlighted && drawHighlighted) || (selected && drawSelected) || (boundary && drawBoundary)))
                continue;

            sf::ConvexShape shape;
            shape.setPointCount(4);
            shape.setPoint(0, {static_cast<float>(rh.a.x), static_cast<float>(rh.a.y)});
            shape.setPoint(1, {static_cast<float>(rh.b.x), static_cast<float>(rh.b.y)});
            shape.setPoint(2, {static_cast<float>(rh.c.x), static_cast<float>(rh.c.y)});
            shape.setPoint(3, {static_cast<float>(rh.d.x), static_cast<float>(rh.d.y)});

            if (highlighted && drawHighlighted)
            {
                shape.setFillColor(sf::Color(255, 0, 0, 50));
                shape.setOutlineColor(sf::Color::Red);
                shape.setOutlineThickness(1.5f);
            }
            else if (selected && drawSelected)
            {
                shape.setFillColor(sf::Color(0, 255, 0, 100));
                shape.setOutlineColor(sf::Color::Green);
            }
            else if (boundary && drawBoundary)
            {
                shape.setFillColor(sf::Color(255, 0, 0, 50));
                shape.setOutlineColor(sf::Color::Red);
                shape.setOutlineThickness(1.5f);
            }
            else if (drawAllRhombi)
            {
                shape.setFillColor(sf::Color(0, 0, 255, 30));
                shape.setOutlineColor(sf::Color::Blue);
            }
            else
            {
                continue;
            }

            window.draw(shape);

            if (drawDots && std::find(model.occupiedRhomus.begin(), model.occupiedRhomus.end(), i) != model.occupiedRhomus.end())
            {
                sf::CircleShape dot(3.f);
                dot.setFillColor(sf::Color::Red);
                dot.setPosition({model.rhombusCenters[i].x - 3.f, model.rhombusCenters[i].y - 3.f});
                window.draw(dot);
            }
        }
    }
}