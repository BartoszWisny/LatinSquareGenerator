#pragma once

#include <stack>

#include "LatinSquare.hpp"
#include "LatinSquareBacktrackingData.hpp"
#include "LatinSquareUpdateData.hpp"

namespace LatinSquareGenerator {
    class LatinSquareGenerator {
        public:
            const LatinSquare generateRandomLatinSquare(const int size);

        private:
            bool checkIfAddToBacktrackingHistory(const Cell& cell) const;

            std::stack<UpdateData> updateHistory_;
            std::stack<BacktrackingData> backtrackingHistory_;
    };
}
