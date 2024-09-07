#pragma once

#include <set>
#include <string>

#include "LatinSquareCell.hpp"

namespace LatinSquareGenerator {
    class TransversalUpdateData {
        public:
            TransversalUpdateData(const Cell& chosenCell, const std::set<std::string>& disabledCellsIds);

            const std::string& getChosenCellId() const;
            const std::set<std::string>& getDisabledCellsIds() const;

        private:
            void setChosenCellId(const std::string& chosenCellId);
            void setDisabledCellsIds(const std::set<std::string>& disabledCellsIds);

            std::string chosenCellId_;
            std::set<std::string> disabledCellsIds_;
    };
}
