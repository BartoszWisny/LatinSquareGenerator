#pragma once

#include <string>
#include <vector>

#include "LatinSquare/Cell.hpp"

namespace Transversal {
    class UpdateData {
        public:
            UpdateData(const LatinSquare::Cell& cell, const std::vector<std::string>& ids);

            const std::string& getChosenCellId() const;
            const std::vector<std::string>& getDisabledCellsIds() const;

        private:
            std::string chosenCellId_;
            std::vector<std::string> disabledCellsIds_;
    };
}
