#pragma once

#include <string>

#include "LatinSquareCell.hpp"
#include "LatinSquareEntropyData.hpp"

namespace LatinSquareGenerator {
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
