#pragma once

#include <string>

#include "LatinSquareCell.hpp"
#include "LatinSquareRegion.hpp"

namespace LatinSquareGenerator {
    class TransversalBacktrackingData {
        public:
            TransversalBacktrackingData(const Region& region, const Cell& chosenCell);

            const std::string& getRegionId() const;
            const std::string& getChosenCellId() const;

        private:
            void setRegionId(const std::string& regionId);
            void setChosenCellId(const std::string& chosenCellId);

            std::string regionId_;
            std::string chosenCellId_;
    };
}
