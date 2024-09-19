#pragma once

#include <set>
#include <string>

#include "Cell.hpp"
#include "EntropyData.hpp"

namespace LatinSquare {
    class UpdateData {
        public:
            UpdateData(const Cell& cell, const EntropyData& entropyData, const std::set<std::string>& ids);

            const std::string& getFilledCellId() const;
            int getFilledCellNumber() const;
            const EntropyData& getPreviousEntropyData() const;
            const std::set<std::string>& getUpdatedCellsIds() const;

        private:
            std::string filledCellId_;
            int filledCellNumber_;
            EntropyData previousEntropyData_;
            std::set<std::string> updatedCellsIds_;
    };
}
