#pragma once

// #include <cstdint>
#include <random>
#include <stack>

#include <boost/multiprecision/cpp_int.hpp>

#include "BacktrackingData.hpp"
#include "Cell.hpp"
#include "LatinSquare.hpp"
#include "UpdateData.hpp"
#include "Types.hpp"

namespace LatinSquare {
    class Generator {
        public:
            const LatinSquare generateRandomLatinSquare(const int size, const Type type, std::mt19937& mersenneTwister);

            const boost::multiprecision::uint512_t /* uint64_t */ countAllLatinSquares(const int size);

        private:
            bool checkIfAddToBacktrackingHistory(const Cell& cell) const;

            std::stack<UpdateData> updateHistory_;
            std::stack<BacktrackingData> backtrackingHistory_;
    };
}
