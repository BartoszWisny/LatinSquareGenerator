#pragma once

#include <functional>
#include <random>
#include <stack>
#include <string>
#include <vector>

#include "BacktrackingData.hpp"
#include "LatinSquare/Cell.hpp"
#include "LatinSquare/LatinSquare.hpp"
#include "UpdateData.hpp"

namespace Transversal {
    class Generator {
        public:
            const std::vector<std::reference_wrapper<LatinSquare::Cell>> findRandomTransversal(
                LatinSquare::LatinSquare& latinSquare, std::mt19937& mersenneTwister);

        private:
            bool checkIfAddToBacktrackingHistory(const LatinSquare::Cell& cell) const;
            bool checkIfRemoveFromBacktrackingHistory(const std::string& regionId) const;

            std::stack<UpdateData> updateHistory_;
            std::stack<BacktrackingData> backtrackingHistory_;
    };
}
