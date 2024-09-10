#include "LatinSquare.hpp"

#include <algorithm>
#include <format>
#include <iterator>

namespace LatinSquareGenerator {
    LatinSquare::LatinSquare() {}

    LatinSquare::LatinSquare(const int size, const bool reduced, const std::mt19937& mersenneTwister) {
        setSize(size);
        setMersenneTwister(mersenneTwister);
        setGrid(reduced);
        shuffleGrid();
    }

    int LatinSquare::getSize() const {
        return size_;
    }

    void LatinSquare::setSize(const int size) {
        size_ = size;
        gridSize_ = size * size;
    }

    void LatinSquare::sortGridByRows() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      if (firstCell.getRow() == secondCell.getRow()) {
                          return firstCell.getColumn() < secondCell.getColumn();
                      } else {
                          return firstCell.getRow() < secondCell.getRow();
                      }
                  });
    }

    void LatinSquare::sortGridByColumns() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      if (firstCell.getColumn() == secondCell.getColumn()) {
                          return firstCell.getRow() < secondCell.getRow();
                      } else {
                          return firstCell.getColumn() < secondCell.getColumn();
                      }
                  });
    }

    void LatinSquare::sortGridByNumbers() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      return firstCell.getNumber() < secondCell.getNumber();
                  });
    }

    const std::vector<Cell>& LatinSquare::getGrid() const {
        return grid_;
    }

    void LatinSquare::setGrid(const bool reduced) {
        for (int index = 0; index < gridSize_; ++index) {
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
                     [this, &ids](const auto& cell) { return ids.contains(cell.getId()); });

        return cells;
    }

    const std::vector<std::reference_wrapper<Region>> LatinSquare::getRegions(const std::set<std::string>& ids) {
        std::vector<std::reference_wrapper<Region>> regions;
        std::copy_if(regions_.begin(), regions_.end(), std::back_inserter(regions),
                     [this, &ids](const auto& region) { return ids.contains(region.getId()); });

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
        return (filledCell.getRow() == cell.getRow()) != (filledCell.getColumn() == cell.getColumn());
    }

    const std::set<std::string> LatinSquare::getUpdatedCellsIds(const Cell& filledCell) {
        std::vector<std::reference_wrapper<Cell>> updatedCells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(updatedCells),
                     [this, &filledCell](const auto& cell) { return checkIfRelatedToFilledCell(filledCell, cell); });
        std::erase_if(updatedCells,
                      [&filledCell](const auto& cellRef) {
                          return !cellRef.get().removeRemainingNumber(filledCell.getNumber());
                      });
        std::set<std::string> updatedCellsIds;
        std::transform(updatedCells.cbegin(), updatedCells.cend(),
                       std::inserter(updatedCellsIds, updatedCellsIds.cend()),
                       [](const auto& cellRef) { return cellRef.get().getId(); });

        return updatedCellsIds;
    }

    const UpdateData LatinSquare::getUpdateData(const Cell& filledCell, const EntropyData& previousEntropyData) {
        return UpdateData(filledCell, previousEntropyData, getUpdatedCellsIds(filledCell));
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

    void LatinSquare::decreaseRelatedRegionsEntropy(
        const std::set<std::string>& relatedRegionsIds, const std::map<std::string, int>& entropyUpdates) {
        const auto relatedRegions = getRegions(relatedRegionsIds);

        for (const auto& regionRef : relatedRegions) {
            auto& region = regionRef.get();
            region.decreaseEntropyBy(entropyUpdates.at(region.getId()));
        }
    }

    const std::set<std::string> LatinSquare::getDisabledCellsIds(const Cell& chosenCell) {
        std::vector<std::reference_wrapper<Cell>> disabledCells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(disabledCells),
                     [this, &chosenCell](const auto& cell) { return checkIfRelatedToChosenCell(chosenCell, cell); });
        std::set<std::string> relatedRegionsIds;
        std::map<std::string, int> entropyUpdates;

        for (const auto& cellRef : disabledCells) {
            auto& cell = cellRef.get();
            cell.disable();

            relatedRegionsIds.insert({cell.getRowId(), cell.getColumnId(), cell.getNumberId()});
            ++entropyUpdates[cell.getRowId()];
            ++entropyUpdates[cell.getColumnId()];
            ++entropyUpdates[cell.getNumberId()];
        }

        decreaseRelatedRegionsEntropy(relatedRegionsIds, entropyUpdates);

        std::set<std::string> disabledCellsIds;
        std::transform(disabledCells.cbegin(), disabledCells.cend(),
                       std::inserter(disabledCellsIds, disabledCellsIds.cend()),
                       [](const auto& cellRef) { return cellRef.get().getId(); });
        disabledCellsIds.erase(chosenCell.getId());

        return disabledCellsIds;
    }

    bool LatinSquare::checkIfRelatedRegion(const Cell& cell, const Region& region) const {
        return cell.getRowId() == region.getId() || cell.getColumnId() == region.getId()
               || cell.getNumberId() == region.getId();
    }

    void LatinSquare::disableRelatedRegions(const Cell& chosenCell) {
        std::vector<std::reference_wrapper<Region>> relatedRegions;
        std::copy_if(regions_.begin(), regions_.end(), std::back_inserter(relatedRegions),
                     [this, &chosenCell](const auto& region) { return checkIfRelatedRegion(chosenCell, region); });

        for (const auto& relatedRegionRef : relatedRegions) {
            relatedRegionRef.get().disable();
        }
    }

    void LatinSquare::increaseRelatedRegionsEntropy(
        const std::set<std::string>& relatedRegionsIds, const std::map<std::string, int>& entropyUpdates) {
        const auto relatedRegions = getRegions(relatedRegionsIds);

        for (const auto& regionRef : relatedRegions) {
            auto& region = regionRef.get();
            regionRef.get().increaseEntropyBy(entropyUpdates.at(region.getId()));
        }
    }

    void LatinSquare::enableCells(const std::set<std::string>& disabledCellsIds) {
        const auto disabledCells = getCells(disabledCellsIds);
        std::set<std::string> relatedRegionsIds;
        std::map<std::string, int> entropyUpdates;

        for (const auto& cellRef : disabledCells) {
            auto& cell = cellRef.get();
            cell.enable();

            relatedRegionsIds.insert({cell.getRowId(), cell.getColumnId(), cell.getNumberId()});
            ++entropyUpdates[cell.getRowId()];
            ++entropyUpdates[cell.getColumnId()];
            ++entropyUpdates[cell.getNumberId()];
        }

        increaseRelatedRegionsEntropy(relatedRegionsIds, entropyUpdates);
    }

    void LatinSquare::enableRelatedRegions(const Cell& chosenCell) {
        std::vector<std::reference_wrapper<Region>> relatedRegions;
        std::copy_if(regions_.begin(), regions_.end(), std::back_inserter(relatedRegions),
                     [this, &chosenCell](const auto& region) { return checkIfRelatedRegion(chosenCell, region); });

        for (const auto& relatedRegionRef : relatedRegions) {
            relatedRegionRef.get().enable();
        }
    }
}
