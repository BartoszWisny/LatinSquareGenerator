#pragma once

#include <functional>
#include <vector>

#include "LatinSquare.hpp"
#include "LatinSquareRegion.hpp"

namespace LatinSquareGenerator {
    class LatinSquareRegions {
        public:
            LatinSquareRegions(LatinSquare& latinSquare);

            const std::vector<Region>& getRegions() const;

            Region& getAvailableRegionWithMinimumEntropy();

        private:
            void setSize(const int size);

            void setRegions(LatinSquare& latinSquare);

            std::vector<Region> regions_;

            int size_;

    };
}
