#pragma once

#include <functional>
#include <vector>

#include "LatinSquare.hpp"
#include "LatinSquareRegion.hpp"

namespace LatinSquareGenerator {
    class LatinSquareRegions {
        public:
            LatinSquareRegions();
            LatinSquareRegions(LatinSquare& latinSquare);

            const std::vector<Region>& getRegions() const;

            Region& getEnabledRegionWithMinimumEntropy();

            const std::vector<std::reference_wrapper<Region>> getRelatedRegions(
                const std::set<std::string>& regionsIds);
            void disableRelatedRegions(const Cell& cell);

        private:
            void setSize(const int size);

            void setRegions(LatinSquare& latinSquare);

            bool checkIfRelatedRegion(const Cell& cell, const Region& region) const;

            std::vector<Region> regions_;

            int size_;

    };
}
