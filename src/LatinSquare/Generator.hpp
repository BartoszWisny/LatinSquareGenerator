#pragma once

#include <random>
#include <stack>

#include "BacktrackingData.hpp"
#include "Cell.hpp"
#include "LatinSquare.hpp"
#include "UpdateData.hpp"

namespace LatinSquare {
    class Generator {
        public:
            const LatinSquare generateRandomLatinSquare(
                const int size, const bool reduced, std::mt19937& mersenneTwister);

        private:
            bool checkIfAddToBacktrackingHistory(const Cell& cell) const;

            std::stack<UpdateData> updateHistory_;
            std::stack<BacktrackingData> backtrackingHistory_;
    };
}
