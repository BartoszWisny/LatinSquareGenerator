#pragma once

#include <set>
#include <string>

#include "LatinSquareCell.hpp"

namespace LatinSquareGenerator {
    class TransversalUpdateData {
        public:
            TransversalUpdateData(const Cell& cell, const std::set<std::string>& ids);

            const std::string& getChosenCellId() const;
            const std::set<std::string>& getDisabledCellsIds() const;

        private:
            void setChosenCellId(const std::string& id);
            void setDisabledCellsIds(const std::set<std::string>& ids);

            std::string chosenCellId_;
            std::set<std::string> disabledCellsIds_;
    };
}
