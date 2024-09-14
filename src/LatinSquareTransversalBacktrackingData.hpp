#pragma once

#include <string>

#include "LatinSquareCell.hpp"
#include "LatinSquareRegion.hpp"

namespace LatinSquareGenerator {
    class TransversalBacktrackingData {
        public:
            TransversalBacktrackingData(const Region& region, const Cell& cell);

            const std::string& getRegionId() const;
            const std::string& getChosenCellId() const;

        private:
            void setRegionId(const std::string& id);
            void setChosenCellId(const std::string& id);

            std::string regionId_;
            std::string chosenCellId_;
    };
}
