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
            void setRegionId(const std::string& id);
            void setChosenCellId(const std::string& id);

            std::string regionId_;
            std::string chosenCellId_;
    };
}
