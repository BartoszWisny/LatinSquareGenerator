#include "LatinSquare.hpp"

#include <algorithm>
#include <format>
#include <iterator>
#include <map>

namespace LatinSquare {
    LatinSquare::LatinSquare() {}

    LatinSquare::LatinSquare(const int size, const bool reduced, const std::mt19937& mersenneTwister) {
        setSize(size);
        setGrid(reduced);
        setMersenneTwister(mersenneTwister);
        shuffleGrid();
    }

    int LatinSquare::getSize() const {
        return size_;
    }

    void LatinSquare::setSize(const int size) {
        size_ = size;
    }

    const std::vector<Cell>& LatinSquare::getGrid() const {
        return grid_;
    }

    void LatinSquare::setGrid(const bool reduced) {
        const auto gridSize = size_ * size_;

        for (int index = 0; index < gridSize; ++index) {
            grid_.emplace_back(Cell(index / size_ + 1, index % size_ + 1, reduced, size_));
        }
    }

    const std::vector<Region>& LatinSquare::getRegions() const {
        return regions_;
    }

    void LatinSquare::setRegions() {
        for (int index = 1; index <= size_; ++index) {
            std::vector<std::reference_wrapper<Cell>> cells;
            std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(cells),
                         [index](const auto& cell) { return cell.getRow() == index; });
            regions_.emplace_back(Region(std::format("R{}", index), size_, cells));

            cells.clear();
            std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(cells),
                         [index](const auto& cell) { return cell.getColumn() == index; });
            regions_.emplace_back(Region(std::format("C{}", index), size_, cells));

            cells.clear();
            std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(cells),
                         [index](const auto& cell) { return cell.getNumber() == index; });
            regions_.emplace_back(Region(std::format("#{}", index), size_, cells));
        }
    }

    void LatinSquare::setMersenneTwister(const std::mt19937& mersenneTwister) {
        mersenneTwister_ = mersenneTwister;
    }

    void LatinSquare::sortGrid() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      return firstCell.getRow() == secondCell.getRow() ?
                             firstCell.getColumn() < secondCell.getColumn() : firstCell.getRow() < secondCell.getRow();
                  });
    }

    void LatinSquare::shuffleGrid() {
        std::shuffle(grid_.begin(), grid_.end(), mersenneTwister_);
    }

    // consider using map for cells - key should be cell id
    Cell& LatinSquare::getCell(const std::string& id) {
        const auto& iterator = std::find_if(
            grid_.begin(), grid_.end(), [&id](const auto& cell) { return cell.getId() == id; });

        return *iterator;
    }

    const std::vector<std::reference_wrapper<Cell>> LatinSquare::getCells(const std::set<std::string>& ids) {
        std::vector<std::reference_wrapper<Cell>> cells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(cells),
                     [&ids](const auto& cell) { return ids.contains(cell.getId()); });

        return cells;
    }

    const std::vector<std::reference_wrapper<Region>> LatinSquare::getRegions(const std::set<std::string>& ids) {
        std::vector<std::reference_wrapper<Region>> regions;
        std::copy_if(regions_.begin(), regions_.end(), std::back_inserter(regions),
                     [&ids](const auto& region) { return ids.contains(region.getId()); });

        return regions;
    }

    bool LatinSquare::checkIfNotFilledCellExists() const {
        return std::any_of(grid_.cbegin(), grid_.cend(), [](const auto& cell) { return cell.getNumber() == 0; });
    }

    Cell& LatinSquare::getNotFilledCellWithMinimumEntropy() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      return firstCell.getEntropy() < secondCell.getEntropy();
                  });
        const auto& iterator = std::find_if(
            grid_.begin(), grid_.end(), [](const auto& cell) { return cell.getNumber() == 0; });

        return *iterator;
    }

    bool LatinSquare::checkIfRelatedToFilledCell(const Cell& filledCell, const Cell& cell) const {
        return (filledCell.getRow() == cell.getRow()) ^ (filledCell.getColumn() == cell.getColumn());
    }

    const std::vector<std::reference_wrapper<Cell>> LatinSquare::getCellsRelatedToFilledCell(const Cell& filledCell) {
        std::vector<std::reference_wrapper<Cell>> cells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(cells),
                     [this, &filledCell](const auto& cell) { return checkIfRelatedToFilledCell(filledCell, cell); });

        return cells;
    }

    void LatinSquare::updateRelatedCells(std::vector<std::reference_wrapper<Cell>>& cells, const int number) {
        std::erase_if(cells, [number](const auto& cellRef) { return !cellRef.get().removeRemainingNumber(number); });
    }

    const std::set<std::string> LatinSquare::getUpdatedCellsIds(
        const std::vector<std::reference_wrapper<Cell>>& cells) {
        std::set<std::string> ids;
        std::transform(cells.cbegin(), cells.cend(), std::inserter(ids, ids.cend()),
                       [](const auto& cellRef) { return cellRef.get().getId(); });

        return ids;
    }

    void LatinSquare::restoreUpdatedCells(const std::vector<std::reference_wrapper<Cell>>& cells, const int number) {
        std::for_each(cells.cbegin(), cells.cend(),
                      [number](const auto& cellRef) { cellRef.get().restoreRemainingNumber(number); });
    }

    Region& LatinSquare::getEnabledRegionWithMinimumEntropy() {
        std::sort(regions_.begin(), regions_.end(),
                  [](const auto& firstRegion, const auto& secondRegion) {
                      return firstRegion.getEntropy() < secondRegion.getEntropy();
                  });
        const auto& iterator = std::find_if(
            regions_.begin(), regions_.end(), [](const auto& region) { return region.isEnabled(); });

        return *iterator;
    }

    bool LatinSquare::checkIfRelatedToChosenCell(const Cell& chosenCell, const Cell& cell) const {
        return cell.isEnabled() && (chosenCell.getRow() == cell.getRow()
               || chosenCell.getColumn() == cell.getColumn() || chosenCell.getNumber() == cell.getNumber());
    }

    const std::vector<std::reference_wrapper<Cell>> LatinSquare::getCellsRelatedToChosenCell(const Cell& chosenCell) {
        std::vector<std::reference_wrapper<Cell>> cells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(cells),
                     [this, &chosenCell](const auto& cell) { return checkIfRelatedToChosenCell(chosenCell, cell); });

        return cells;
    }

    void LatinSquare::disableRelatedCells(const std::vector<std::reference_wrapper<Cell>>& cells) {
        std::for_each(cells.cbegin(), cells.cend(), [](const auto& cellRef) { cellRef.get().disable(); });
    }

    void LatinSquare::decreaseRelatedRegionsEntropy(const std::vector<std::reference_wrapper<Cell>>& cells) {
        std::set<std::string> regionsIds;
        std::map<std::string, int> entropyUpdates;

        for (const auto& cellRef : cells) {
            const auto& cell = cellRef.get();

            regionsIds.insert({cell.getRowId(), cell.getColumnId(), cell.getNumberId()});
            ++entropyUpdates[cell.getRowId()];
            ++entropyUpdates[cell.getColumnId()];
            ++entropyUpdates[cell.getNumberId()];
        }

        const auto regions = getRegions(regionsIds);

        for (const auto& regionRef : regions) {
            auto& region = regionRef.get();

            region.decreaseEntropyBy(entropyUpdates.at(region.getId()));
        }
    }

    const std::set<std::string> LatinSquare::getDisabledCellsIds(
        const std::vector<std::reference_wrapper<Cell>>& cells, const Cell& cell) {
        std::set<std::string> ids;
        std::transform(cells.cbegin(), cells.cend(), std::inserter(ids, ids.cend()),
                       [](const auto& cellRef) { return cellRef.get().getId(); });
        ids.erase(cell.getId());

        return ids;
    }

    bool LatinSquare::checkIfRelatedRegion(const Cell& cell, const Region& region) const {
        return cell.getRowId() == region.getId() || cell.getColumnId() == region.getId()
               || cell.getNumberId() == region.getId();
    }

    const std::vector<std::reference_wrapper<Region>> LatinSquare::getRelatedRegions(const Cell& cell) {
        std::vector<std::reference_wrapper<Region>> regions;
        std::copy_if(regions_.begin(), regions_.end(), std::back_inserter(regions),
                     [this, &cell](const auto& region) { return checkIfRelatedRegion(cell, region); });

        return regions;
    }

    void LatinSquare::disableRelatedRegions(const std::vector<std::reference_wrapper<Region>>& regions) {
        std::for_each(regions.cbegin(), regions.cend(), [](const auto& regionRef) { regionRef.get().disable(); });
    }

    void LatinSquare::enableDisabledCells(const std::vector<std::reference_wrapper<Cell>>& cells) {
        std::for_each(cells.cbegin(), cells.cend(), [](const auto& cellRef) { cellRef.get().enable(); });
    }

    void LatinSquare::increaseRelatedRegionsEntropy(const std::vector<std::reference_wrapper<Cell>>& cells) {
        std::set<std::string> regionsIds;
        std::map<std::string, int> entropyUpdates;

        for (const auto& cellRef : cells) {
            const auto& cell = cellRef.get();

            regionsIds.insert({cell.getRowId(), cell.getColumnId(), cell.getNumberId()});
            ++entropyUpdates[cell.getRowId()];
            ++entropyUpdates[cell.getColumnId()];
            ++entropyUpdates[cell.getNumberId()];
        }

        const auto regions = getRegions(regionsIds);

        for (const auto& regionRef : regions) {
            auto& region = regionRef.get();

            region.increaseEntropyBy(entropyUpdates.at(region.getId()));
        }
    }

    void LatinSquare::enableRelatedRegions(const std::vector<std::reference_wrapper<Region>>& regions) {
        std::for_each(regions.cbegin(), regions.cend(), [](const auto& regionRef) { regionRef.get().enable(); });
    }
}
