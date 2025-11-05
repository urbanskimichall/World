#pragma once
#include "GridModel.hpp"

namespace grid
{
    class GridGenerator
    {
    public:
        explicit GridGenerator(double spacing = 5.0);
        GridModel generate(uint32_t numRows, uint32_t numCols);

    private:
        void generateGridPoints(GridModel &model, uint32_t numRows, uint32_t numCols,
                                double diagX, double diagY);
        void generateRhombi(GridModel &model);

        std::optional<Rhombus> generateSingleRhombus(const Node &node);
        std::vector<utils::Point> generateNeighbourCenters(const utils::Point &center, float shift);

        double spacing;
        static constexpr float SHIFT = 25.f;
    };
}