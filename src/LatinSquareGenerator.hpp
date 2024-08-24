#pragma once

#include <stack>

#include "LatinSquare.hpp"
#include "LatinSquareUpdateData.hpp"

namespace LatinSquareGenerator {
    class LatinSquareGenerator {
        public:
            const LatinSquare generateRandomLatinSquare(const int size);

        private:
            std::stack<UpdateData> updateHistory_;
    };
}
