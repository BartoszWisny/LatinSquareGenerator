#include "LatinSquare.hpp"

#include <algorithm>
#include <iterator>
#include <map>

namespace LatinSquareGenerator {
    LatinSquare::LatinSquare() {}

    LatinSquare::LatinSquare(const int size, const std::mt19937& mersenneTwister) {
        setSize(size);
        setMersenneTwister(mersenneTwister);
        reset();
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

    const std::vector<Cell>& LatinSquare::getGrid() {
        return grid_;
    }

    int LatinSquare::getGridSize() const {
        return gridSize_;
    }

    void LatinSquare::setMersenneTwister(const std::mt19937& mersenneTwister) {
        mersenneTwister_ = mersenneTwister;
    }

    void LatinSquare::reset() {
        if (grid_.empty()) {
            for (int index = 0; index < gridSize_; index++) {
                grid_.emplace_back(Cell(index / size_ + 1, index % size_ + 1, size_));
            }
        } else {
            std::for_each(grid_.begin(), grid_.end(), [](auto& cell) { cell.reset(); });
        }

        std::shuffle(grid_.begin(), grid_.end(), mersenneTwister_);
    }

    Cell& LatinSquare::getCell(const std::string& id) {
        const auto& iterator = std::find_if(
            grid_.begin(), grid_.end(), [&id](const auto& cell) { return cell.getId() == id; });

        return *iterator;
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

    const std::vector<std::reference_wrapper<Cell>> LatinSquare::getPreviousUpdatedCells(
        const std::set<std::string>& updatedCellsIds) {
        std::vector<std::reference_wrapper<Cell>> previousUpdatedCells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(previousUpdatedCells),
                     [this, &updatedCellsIds](const auto& cell) { return updatedCellsIds.contains(cell.getId()); });

        return previousUpdatedCells;
    }

    // here chosen cell is also included (maybe it should be changed?)
    bool LatinSquare::checkIfRelatedToChosenCell(const Cell& chosenCell, const Cell& cell) const {
        return chosenCell.isEnabled() && (chosenCell.getRow() == cell.getRow()
               || chosenCell.getColumn() == cell.getColumn() || chosenCell.getNumber() == cell.getNumber());
    }

    // can be done better:
    // 1) regions as part of LatinSquare
    // 2) maybe for chosen cell disable 3 regions and set entropy to 0, then analyze rest of the related cells
    const std::set<std::string> LatinSquare::disableRelatedCells(
        const Cell& chosenCell, LatinSquareRegions& regions) {
        std::vector<std::reference_wrapper<Cell>> relatedCells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(relatedCells),
                     [this, &chosenCell](const auto& cell) { return checkIfRelatedToChosenCell(chosenCell, cell); });
        std::map<std::string, int> entropyUpdates;

        for (const auto& cellRef : relatedCells) {
            auto& cell = cellRef.get();
            cell.disable();

            // 3 options - check which one is the fastest:
            // 1) only disable related regions
            // ++relatedRegions[cell.getRowId()];
            // ++relatedRegions[cell.getColumnId()];
            // ++relatedRegions[cell.getNumberId()];

            // 2) disable related regions and set entropy to 0 - 1st option
            // if (chosenCell.getRow() == cell.getRow()) {
            //     ++relatedRegions[cell.getColumnId()];
            //     ++relatedRegions[cell.getNumberId()];
            // }
            // if (chosenCell.getColumn() == cell.getColumn()) {
            //     ++relatedRegions[cell.getRowId()];
            //     ++relatedRegions[cell.getNumberId()];
            // }
            // if (chosenCell.getNumber() == cell.getNumber()) {
            //     ++relatedRegions[cell.getRowId()];
            //     ++relatedRegions[cell.getColumnId()];
            // }

            // 3) disable related regions and set entropy to 0 - 2nd option
            // if (chosenCell.getRow() != cell.getRow()) {
            //     ++relatedRegions[cell.getRowId()];
            // }
            // if (chosenCell.getColumn() != cell.getColumn()) {
            //     ++relatedRegions[cell.getColumnId()];
            // }
            // if (chosenCell.getNumber() != cell.getNumber()) {
            //     ++relatedRegions[cell.getNumberId()];
            // }

            ++entropyUpdates.at(cell.getRowId());
            ++entropyUpdates.at(cell.getColumnId());
            ++entropyUpdates.at(cell.getNumberId());
        }

        std::set<std::string> regionsIds;

        for (const auto& entropyUpdate : entropyUpdates) {
            regionsIds.insert(entropyUpdate.first);
        }

        const auto relatedRegions = regions.getRelatedRegions(regionsIds);

        for (const auto& regionRef : relatedRegions) {
            auto& region = regionRef.get();
            region.decreaseEntropyBy(entropyUpdates.at(region.getId()));
        }

        std::set<std::string> relatedCellsIds;
        std::transform(relatedCells.cbegin(), relatedCells.cend(),
                       std::inserter(relatedCellsIds, relatedCellsIds.cend()),
                       [](const auto& cellRef) { return cellRef.get().getId(); });

        return relatedCellsIds;
    }
}
