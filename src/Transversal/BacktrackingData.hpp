#pragma once

#include <string>

#include "LatinSquare/Cell.hpp"
#include "LatinSquare/Region.hpp"

namespace Transversal {
    class BacktrackingData {
        public:
            BacktrackingData(const LatinSquare::Region& region, const LatinSquare::Cell& cell);

            const std::string& getRegionId() const;
            const std::string& getChosenCellId() const;

        private:
            std::string regionId_, chosenCellId_;
    };
}
