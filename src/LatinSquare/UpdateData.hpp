#pragma once

#include <string>
#include <vector>

#include "Cell.hpp"
#include "EntropyData.hpp"

namespace LatinSquare {
    class UpdateData {
        public:
            UpdateData(const Cell& cell, const EntropyData& entropyData, const std::vector<std::string>& ids);

            const std::string& getFilledCellId() const;
            int getFilledCellNumber() const;
            const EntropyData& getPreviousEntropyData() const;
            const std::vector<std::string>& getUpdatedCellsIds() const;

        private:
            std::string filledCellId_;
            int filledCellNumber_;
            EntropyData previousEntropyData_;
            std::vector<std::string> updatedCellsIds_;
    };
}
