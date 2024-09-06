#pragma once

#include <functional>
#include <stack>
#include <vector>

#include "LatinSquare.hpp"
#include "LatinSquareRegion.hpp"
#include "LatinSquareTransversalUpdateData.hpp"

namespace LatinSquareGenerator {
    class LatinSquareTransversalGenerator {
        public:
            const std::vector<std::reference_wrapper<Cell>> findRandomTransversal(LatinSquare& latinSquare);

        private:
            std::stack<TransversalUpdateData> updateHistory_;
    };
}
