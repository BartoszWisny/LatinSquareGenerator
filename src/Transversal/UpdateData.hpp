#pragma once

#include <set>
#include <string>

#include "LatinSquare/Cell.hpp"

namespace Transversal {
    class UpdateData {
        public:
            UpdateData(const LatinSquare::Cell& cell, const std::set<std::string>& ids);

            const std::string& getChosenCellId() const;
            const std::set<std::string>& getDisabledCellsIds() const;

        private:
            std::string chosenCellId_;
            std::set<std::string> disabledCellsIds_;
    };
}
