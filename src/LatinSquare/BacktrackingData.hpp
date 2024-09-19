#pragma once

#include <string>

#include "Cell.hpp"
#include "EntropyData.hpp"

namespace LatinSquare {
    class BacktrackingData {
        public:
            BacktrackingData(const Cell& cell, const EntropyData& entropyData);

            const std::string& getFilledCellId() const;
            const EntropyData& getPreviousEntropyData() const;

        private:
            std::string filledCellId_;
            EntropyData previousEntropyData_;
    };
}
