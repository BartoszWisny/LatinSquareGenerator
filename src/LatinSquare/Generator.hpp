#pragma once

#include <random>
#include <stack>

#include "BacktrackingData.hpp"
#include "Cell.hpp"
#include "LatinSquare.hpp"
#include "UpdateData.hpp"
#include "Types.hpp"

namespace LatinSquare {
    class Generator {
        public:
            const LatinSquare generateRandomLatinSquare(const int size, const Type type, std::mt19937& mersenneTwister);

        private:
            bool checkIfAddToBacktrackingHistory(const Cell& cell) const;

            std::stack<UpdateData> updateHistory_;
            std::stack<BacktrackingData> backtrackingHistory_;
    };
}
