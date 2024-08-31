#pragma once

#include <functional>
#include <vector>

#include "LatinSquare.hpp"
#include "LatinSquareRegion.hpp"

namespace LatinSquareGenerator {
    class LatinSquareTransversalGenerator {
        public:
            const std::vector<std::reference_wrapper<Cell>> findRandomTransversal(LatinSquare& latinSquare);

        private:


            // bool checkIfRelatedCell(const Cell& filledCell, const Cell& cell) const;
            // std::stack<> updateHistory_;
    };
}
