#pragma once

#include <string>

#include "LatinSquareCell.hpp"
#include "LatinSquareEntropyData.hpp"

namespace LatinSquareGenerator {
    class BacktrackingData {
        public:
            BacktrackingData(const Cell& filledCell, const EntropyData& previousEntropyData);

            const std::string& getFilledCellId() const;
            const EntropyData& getPreviousEntropyData() const;

        private:
            void setFilledCellId(const std::string& filledCellId);
            void setPreviousEntropyData(const EntropyData& previousEntropyData);

            std::string filledCellId_;
            EntropyData previousEntropyData_;
    };
}
