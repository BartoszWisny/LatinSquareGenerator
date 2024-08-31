#include "LatinSquareRegions.hpp"

#include <algorithm>
#include <format>

namespace LatinSquareGenerator {
    LatinSquareRegions::LatinSquareRegions(LatinSquare& latinSquare) {
        setSize(latinSquare.getSize());
        setRegions(latinSquare);
    }

    void LatinSquareRegions::setSize(const int size) {
        size_ = size;
    }

    const std::vector<Region>& LatinSquareRegions::getRegions() const {
        return regions_;
    }

    void LatinSquareRegions::setRegions(LatinSquare& latinSquare) {
        const auto& grid = latinSquare.getGrid();

        latinSquare.sortGridByRows();
        auto iterator = grid.cbegin();
        int index = 0;

        while (iterator != grid.cend()) {
            std::vector<std::reference_wrapper<Cell>> cells;
            std::copy(iterator, iterator + size_, cells.begin());
            regions_.emplace_back(Region(std::format("R{}", ++index), size_, cells));
        }

        latinSquare.sortGridByColumns();
        iterator = grid.cbegin();
        index = 0;

        while (iterator != grid.cend()) {
            std::vector<std::reference_wrapper<Cell>> cells;
            std::copy(iterator, iterator + size_, cells.begin());
            regions_.emplace_back(Region(std::format("C{}", ++index), size_, cells));
        }

        latinSquare.sortGridByNumbers();
        iterator = grid.cbegin();
        index = 0;

        while (iterator != grid.cend()) {
            std::vector<std::reference_wrapper<Cell>> cells;
            std::copy(iterator, iterator + size_, cells.begin());
            regions_.emplace_back(Region(std::format("#{}", ++index), size_, cells));
        }
    }

    Region& LatinSquareRegions::getAvailableRegionWithMinimumEntropy() {
        std::sort(regions_.begin(), regions_.end(),
                  [](const auto& firstRegion, const auto& secondRegion) {
                      return firstRegion.getEntropy() < secondRegion.getEntropy();
                  });
        const auto& iterator = std::find_if(
            regions_.begin(), regions_.end(), [](const auto& region) { return region.isAvailable(); });

        return *iterator;
    }
}
