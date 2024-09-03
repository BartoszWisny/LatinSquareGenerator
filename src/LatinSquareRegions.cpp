#include "LatinSquareRegions.hpp"

#include <algorithm>
#include <format>
#include <iterator>

namespace LatinSquareGenerator {
    LatinSquareRegions::LatinSquareRegions() {}

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
            std::advance(iterator, size_);
        }

        latinSquare.sortGridByColumns();
        iterator = grid.cbegin();
        index = 0;

        while (iterator != grid.cend()) {
            std::vector<std::reference_wrapper<Cell>> cells;
            std::copy(iterator, iterator + size_, cells.begin());
            regions_.emplace_back(Region(std::format("C{}", ++index), size_, cells));
            std::advance(iterator, size_);
        }

        latinSquare.sortGridByNumbers();
        iterator = grid.cbegin();
        index = 0;

        while (iterator != grid.cend()) {
            std::vector<std::reference_wrapper<Cell>> cells;
            std::copy(iterator, iterator + size_, cells.begin());
            regions_.emplace_back(Region(std::format("#{}", ++index), size_, cells));
            std::advance(iterator, size_);
        }
    }

    Region& LatinSquareRegions::getEnabledRegionWithMinimumEntropy() {
        std::sort(regions_.begin(), regions_.end(),
                  [](const auto& firstRegion, const auto& secondRegion) {
                      return firstRegion.getEntropy() < secondRegion.getEntropy();
                  });
        const auto& iterator = std::find_if(
            regions_.begin(), regions_.end(), [](const auto& region) { return region.isEnabled(); });

        return *iterator;
    }

    const std::vector<std::reference_wrapper<Region>> LatinSquareRegions::getRelatedRegions(
        const std::set<std::string>& regionsIds) {
        std::vector<std::reference_wrapper<Region>> relatedRegions;
        std::copy_if(regions_.begin(), regions_.end(), std::back_inserter(relatedRegions),
                     [this, &regionsIds](const auto& region) { return regionsIds.contains(region.getId()); });

        return relatedRegions;
    }

    bool LatinSquareRegions::checkIfRelatedRegion(const Cell& cell, const Region& region) const {
        return cell.getRowId() == region.getId() || cell.getColumnId() == region.getId()
               || cell.getNumberId() == region.getId();
    }

    void LatinSquareRegions::disableRelatedRegions(const Cell& cell) {
        std::vector<std::reference_wrapper<Region>> relatedRegions;
        std::copy_if(regions_.begin(), regions_.end(), std::back_inserter(relatedRegions),
                     [this, &cell](const auto& region) { return checkIfRelatedRegion(cell, region); });

        for (const auto& relatedRegionRef : relatedRegions) {
            relatedRegionRef.get().disable();
        }
    }
}
