#pragma once

#include <functional>
#include <stack>
#include <vector>

#include "LatinSquare.hpp"
#include "LatinSquareRegion.hpp"
#include "LatinSquareTransversalBacktrackingData.hpp"
#include "LatinSquareTransversalUpdateData.hpp"

namespace LatinSquareGenerator {
    class LatinSquareTransversalGenerator {
        public:
            const std::vector<std::reference_wrapper<Cell>> findRandomTransversal(LatinSquare& latinSquare);

        private:
            bool checkIfAddToBacktrackingHistory(const Cell& cell) const;
            bool checkIfRemoveFromBacktrackingHistory(const std::string& regionId) const;

            std::stack<TransversalUpdateData> updateHistory_;
            std::stack<TransversalBacktrackingData> backtrackingHistory_;
    };
}
