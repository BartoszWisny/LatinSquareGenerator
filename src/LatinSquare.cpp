#include "LatinSquare.hpp"

#include <algorithm>
#include <iterator>

namespace LatinSquareGenerator {
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

    const std::vector<Cell>& LatinSquare::getGrid() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      if (firstCell.getRow() == secondCell.getRow()) {
                          return firstCell.getColumn() < secondCell.getColumn();
                      } else {
                          return firstCell.getRow() < secondCell.getRow();
                      }
                  });

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

    bool LatinSquare::checkIfRelatedCell(const Cell& filledCell, const Cell& cell) const {
        return (filledCell.getRow() == cell.getRow()) != (filledCell.getColumn() == cell.getColumn());
    }

    const std::set<std::string> LatinSquare::getUpdatedCellsIds(const Cell& filledCell) {
        std::vector<std::reference_wrapper<Cell>> updatedCells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(updatedCells),
                     [this, &filledCell](const auto& cell) { return checkIfRelatedCell(filledCell, cell); });
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
}
