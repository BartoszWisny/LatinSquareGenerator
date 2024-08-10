#pragma once

#include <stack>

#include "LatinSquare.hpp"
#include "LatinSquareUtils.hpp"

namespace LatinSquareGenerator {
    class LatinSquareGenerator {
        public:
            LatinSquare generateRandomLatinSquare(int size);

        private:
            std::stack<UpdateData> updateHistory;
    };
}
