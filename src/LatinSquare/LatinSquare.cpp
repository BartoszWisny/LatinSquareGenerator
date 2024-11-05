#include "LatinSquare.hpp"

#include <algorithm>
#include <limits>

#include <cpp/iterator.hpp>

namespace LatinSquare {
    LatinSquare::LatinSquare() {}

    LatinSquare::LatinSquare(const int size, const Type type)
        : size_(size) {
        setGrid(type);
    }

    LatinSquare::LatinSquare(const int size, const Type type, std::mt19937& mersenneTwister)
        : size_(size), mersenneTwister_(mersenneTwister) {
        setGrid(type);
    }

    int LatinSquare::getSize() const {
        return size_;
    }

    const std::vector<Cell>& LatinSquare::getGrid() const {
        return grid_;
    }

    void LatinSquare::setGrid(const Type type) {
        const auto gridSize = size_ * size_;
        grid_.reserve(gridSize);

        for (int index = 0; index < gridSize; ++index) {
            grid_.emplace_back(index / size_, index % size_, type, size_);
        }
    }

    void LatinSquare::setRegions() {
        regions_.reserve(3 * size_);

        std::vector<std::reference_wrapper<Cell>> rowCells, columnCells, numberCells;
        rowCells.reserve(size_);
        columnCells.reserve(size_);
        numberCells.reserve(size_);

        const auto rowRegion = 'R', columnRegion = 'C', numberRegion = '#';
        std::string indexString, rowId, columnId, numberId;

        for (int index = 0; index < size_; ++index) {
            rowCells.clear();
            columnCells.clear();
            numberCells.clear();

            for (auto& cell : grid_) {
                if (cell.getRow() == index) {
                    rowCells.emplace_back(cell);
                }

                if (cell.getColumn() == index) {
                    columnCells.emplace_back(cell);
                }

                if (cell.getNumber() == index) {
                    numberCells.emplace_back(cell);
                }
            }

            indexString = std::to_string(index);
            rowId = rowRegion;
            rowId += indexString;
            columnId = columnRegion;
            columnId += indexString;
            numberId = numberRegion;
            numberId += indexString;

            regions_.emplace_back(rowId, size_, rowCells);
            regions_.emplace_back(columnId, size_, columnCells);
            regions_.emplace_back(numberId, size_, numberCells);
        }
    }

    void LatinSquare::sortGrid() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      return firstCell.getRow() == secondCell.getRow() ?
                             firstCell.getColumn() < secondCell.getColumn() : firstCell.getRow() < secondCell.getRow();
                  });
    }

    Cell& LatinSquare::getCell(const std::string& id) {
        const auto& iterator = std::find_if(
            grid_.begin(), grid_.end(), [&id](const auto& cell) { return cell.getId() == id; });

        return *iterator;
    }

    const std::vector<std::reference_wrapper<Cell>> LatinSquare::getCells(const std::vector<std::string>& ids) {
        std::vector<std::reference_wrapper<Cell>> cells;
        cells.reserve(ids.size());

        for (auto& cell : grid_) {
            if (std::find(ids.cbegin(), ids.cend(), cell.getId()) != ids.cend()) {
                cells.emplace_back(cell);
            }
        }

        return cells;
    }

    const std::vector<std::reference_wrapper<Region>> LatinSquare::getRegions(const std::vector<std::string>& ids) {
        std::vector<std::reference_wrapper<Region>> regions;
        regions.reserve(ids.size());

        for (auto& region : regions_) {
            if (std::find(ids.cbegin(), ids.cend(), region.getId()) != ids.cend()) {
                regions.emplace_back(region);
            }
        }

        return regions;
    }

    bool LatinSquare::checkIfNotFilledCellExists() const {
        return std::any_of(grid_.cbegin(), grid_.cend(), [](const auto& cell) { return !cell.isFilled(); });
    }

    Cell& LatinSquare::getNotFilledCellWithMinimumEntropy() {
        Cell* iterator = nullptr;
        auto minEntropy = std::numeric_limits<int>::max();
        int entropy;

        for (auto& cell : grid_) {
            if (!cell.isFilled()) {
                entropy = cell.getEntropy();

                if (entropy < minEntropy) {
                    minEntropy = entropy;
                    iterator = &cell;
                }
            }
        }

        return *iterator;
    }

    Cell& LatinSquare::getRandomNotFilledCellWithMinimumEntropy() {
        Cell* iterator = nullptr;
        auto minEntropy = std::numeric_limits<int>::max();
        int entropy;

        for (auto& cell : grid_) {
            if (!cell.isFilled()) {
                entropy = cell.getEntropy();

                if (entropy < minEntropy) {
                    minEntropy = entropy;
                    iterator = &cell;
                } else if (entropy == minEntropy) {
                    if (mersenneTwister_() & 1) {
                        iterator = &cell;
                    }
                }
            }
        }

        return *iterator;
    }

    bool LatinSquare::checkIfNotFilledAndRelatedToFilledCell(const Cell& filledCell, const Cell& cell) const {
        return !cell.isFilled()
               && ((filledCell.getRow() == cell.getRow()) ^ (filledCell.getColumn() == cell.getColumn()));
    }

    const std::vector<std::reference_wrapper<Cell>> LatinSquare::getCellsRelatedToFilledCell(const Cell& filledCell) {
        std::vector<std::reference_wrapper<Cell>> cells;
        cells.reserve(2 * size_ - 2);

        std::copy_if(grid_.begin(), grid_.end(), cpp::back_emplacer(cells),
                     [this, &filledCell](const auto& cell) {
                         return checkIfNotFilledAndRelatedToFilledCell(filledCell, cell);
                     });

        return cells;
    }

    void LatinSquare::updateRelatedCells(std::vector<std::reference_wrapper<Cell>>& cells, const int number) {
        cells.erase(std::remove_if(cells.begin(), cells.end(),
                                   [number](const auto& cellRef) {
                                       return !cellRef.get().removeRemainingNumber(number);
                                   }), cells.end());
    }

    const std::vector<std::string> LatinSquare::getUpdatedCellsIds(
        const std::vector<std::reference_wrapper<Cell>>& cells) {
        std::vector<std::string> ids;
        ids.reserve(cells.size());

        for (const auto& cellRef : cells) {
            ids.emplace_back(cellRef.get().getId());
        }

        return ids;
    }

    void LatinSquare::restoreUpdatedCells(const std::vector<std::reference_wrapper<Cell>>& cells, const int number) {
        std::for_each(cells.cbegin(), cells.cend(),
                      [number](const auto& cellRef) { cellRef.get().restoreRemainingNumber(number); });
    }

    Region& LatinSquare::getEnabledRegionWithMinimumEntropy() {
        Region* iterator = nullptr;
        auto minEntropy = std::numeric_limits<int>::max();
        int entropy;

        for (auto& region : regions_) {
            if (region.isEnabled()) {
                entropy = region.getEntropy();

                if (entropy < minEntropy) {
                    minEntropy = entropy;
                    iterator = &region;
                }
            }
        }

        return *iterator;
    }

    Region& LatinSquare::getRandomEnabledRegionWithMinimumEntropy() {
        Region* iterator = nullptr;
        auto minEntropy = std::numeric_limits<int>::max();
        int entropy;

        for (auto& region : regions_) {
            if (region.isEnabled()) {
                entropy = region.getEntropy();

                if (entropy < minEntropy) {
                    minEntropy = entropy;
                    iterator = &region;
                } else if (entropy == minEntropy) {
                    if (mersenneTwister_() & 1) {
                        iterator = &region;
                    }
                }
            }
        }

        return *iterator;
    }

    bool LatinSquare::checkIfEnabledAndRelatedToChosenCell(const Cell& chosenCell, const Cell& cell) const {
        return cell.isEnabled() && (chosenCell.getRow() == cell.getRow()
               || chosenCell.getColumn() == cell.getColumn() || chosenCell.getNumber() == cell.getNumber());
    }

    const std::vector<std::reference_wrapper<Cell>> LatinSquare::getCellsRelatedToChosenCell(const Cell& chosenCell) {
        std::vector<std::reference_wrapper<Cell>> cells;
        cells.reserve(3 * size_ - 2);

        std::copy_if(grid_.begin(), grid_.end(), cpp::back_emplacer(cells),
                     [this, &chosenCell](const auto& cell) {
                         return checkIfEnabledAndRelatedToChosenCell(chosenCell, cell);
                     });

        return cells;
    }

    void LatinSquare::disableRelatedCells(const std::vector<std::reference_wrapper<Cell>>& cells) {
        std::for_each(cells.cbegin(), cells.cend(), [](const auto& cellRef) { cellRef.get().disable(); });
    }

    void LatinSquare::decreaseRelatedRegionsEntropy(const std::vector<std::reference_wrapper<Cell>>& cells) {
        std::vector<std::string> regionsIds;
        regionsIds.reserve(3 * size_);
        std::vector<bool> usedIds(3 * size_, false); // consider using bitset here
        std::vector<int> entropyUpdates(3 * size_, 0);
        int rowIdAsInt, columnIdAsInt, numberIdAsInt;
        Cell* cell = nullptr;
        Region* region = nullptr;

        auto addToRegionsIds = [&regionsIds, &usedIds](const auto& id, const auto idAsInt) {
                                   if (!usedIds[idAsInt]) {
                                       regionsIds.emplace_back(id);
                                       usedIds[idAsInt] = true;
                                   }
                               };

        for (const auto& cellRef : cells) {
            cell = &cellRef.get();
            rowIdAsInt = (*cell).getRowIdAsInt();
            columnIdAsInt = (*cell).getColumnIdAsInt();
            numberIdAsInt = (*cell).getNumberIdAsInt();

            addToRegionsIds((*cell).getRowId(), rowIdAsInt);
            addToRegionsIds((*cell).getColumnId(), columnIdAsInt);
            addToRegionsIds((*cell).getNumberId(), numberIdAsInt);
            ++entropyUpdates[rowIdAsInt];
            ++entropyUpdates[columnIdAsInt];
            ++entropyUpdates[numberIdAsInt];
        }

        const auto regions = getRegions(regionsIds);

        for (const auto& regionRef : regions) {
            region = &regionRef.get();

            (*region).decreaseEntropyBy(entropyUpdates[(*region).getIdAsInt()]);
        }
    }

    const std::vector<std::string> LatinSquare::getDisabledCellsIds(
        const std::vector<std::reference_wrapper<Cell>>& cells, const Cell& chosenCell) {
        std::vector<std::string> ids;
        ids.reserve(cells.size() - 1);
        Cell* cell = nullptr;

        for (const auto& cellRef : cells) {
            cell = &cellRef.get();

            if ((*cell).getId() != chosenCell.getId()) {
                ids.emplace_back((*cell).getId());
            }
        }

        return ids;
    }

    bool LatinSquare::checkIfRelatedRegion(const Cell& cell, const Region& region) const {
        return cell.getRowId() == region.getId() || cell.getColumnId() == region.getId()
               || cell.getNumberId() == region.getId();
    }

    const std::vector<std::reference_wrapper<Region>> LatinSquare::getRelatedRegions(const Cell& cell) {
        std::vector<std::reference_wrapper<Region>> regions;
        regions.reserve(3);

        std::copy_if(regions_.begin(), regions_.end(), cpp::back_emplacer(regions),
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
        std::vector<std::string> regionsIds;
        regionsIds.reserve(3 * size_);
        std::vector<bool> usedIds(3 * size_, false); // consider using bitset here
        std::vector<int> entropyUpdates(3 * size_, 0);
        int rowIdAsInt, columnIdAsInt, numberIdAsInt;
        Cell* cell = nullptr;
        Region* region = nullptr;

        auto addToRegionsIds = [&regionsIds, &usedIds](const std::string& id, int idAsInt) {
                                   if (!usedIds[idAsInt]) {
                                       regionsIds.emplace_back(id);
                                       usedIds[idAsInt] = true;
                                   }
                               };

        for (const auto& cellRef : cells) {
            cell = &cellRef.get();
            rowIdAsInt = (*cell).getRowIdAsInt();
            columnIdAsInt = (*cell).getColumnIdAsInt();
            numberIdAsInt = (*cell).getNumberIdAsInt();

            addToRegionsIds((*cell).getRowId(), rowIdAsInt);
            addToRegionsIds((*cell).getColumnId(), columnIdAsInt);
            addToRegionsIds((*cell).getNumberId(), numberIdAsInt);
            ++entropyUpdates[rowIdAsInt];
            ++entropyUpdates[columnIdAsInt];
            ++entropyUpdates[numberIdAsInt];
        }

        const auto regions = getRegions(regionsIds);

        for (const auto& regionRef : regions) {
            region = &regionRef.get();

            (*region).increaseEntropyBy(entropyUpdates[(*region).getIdAsInt()]);
        }
    }

    void LatinSquare::enableRelatedRegions(const std::vector<std::reference_wrapper<Region>>& regions) {
        std::for_each(regions.cbegin(), regions.cend(), [](const auto& regionRef) { regionRef.get().enable(); });
    }
}
