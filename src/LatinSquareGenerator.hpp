#pragma once

#include <stack>

#include "LatinSquare.hpp"
#include "LatinSquareUtils.hpp"

namespace LatinSquareGenerator {
    class LatinSquareGenerator {
        public:
            LatinSquareGenerator();
            const LatinSquare generateRandomLatinSquare(const int size);

        private:
            std::stack<UpdateData> updateHistory_;
    };
}
