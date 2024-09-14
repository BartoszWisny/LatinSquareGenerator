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
            void setFilledCellId(const std::string& id);
            void setPreviousEntropyData(const EntropyData& entropyData);

            std::string filledCellId_;
            EntropyData previousEntropyData_;
    };
}
