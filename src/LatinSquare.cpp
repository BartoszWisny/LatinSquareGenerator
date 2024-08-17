#include "LatinSquare.hpp"

#include <algorithm>

namespace LatinSquareGenerator {
    LatinSquare::LatinSquare(const int size) {
        setSize(size);
        reset();
    }

    void LatinSquare::setSize(const int size) {
        size_ = size;
        gridSize_ = size * size;
    }

    const std::vector<Cell>& LatinSquare::getGrid() {
        std::sort(grid_.begin(), grid_.end(),
            [](const auto& firstCell, const auto& secondCell) {
                if (firstCell.getRow() != secondCell.getRow()) {
                    return firstCell.getRow() < secondCell.getRow();
                } else {
                    return firstCell.getColumn() < secondCell.getColumn();
                }
            });

        return grid_;
    }

    Cell& LatinSquare::getCell(const std::string& id) { // TODO: should return ref to cell from grid
        const auto& iterator = std::find_if(
            grid_.begin(), grid_.end(), [&id](const auto& cell) { return cell.getId() == id; });

        return *iterator;
    }

    int LatinSquare::calculateCellRow(const int index) const {
        return index / size_ + 1;
    }

    int LatinSquare::calculateCellColumn(const int index) const {
        return index % size_ + 1;
    }

    void LatinSquare::reset() {
        grid_.clear();

        for (int index = 0; index < gridSize_; index++) {
            grid_.push_back(Cell(calculateCellRow(index), calculateCellColumn(index), size_));
        }
    }

    bool LatinSquare::checkIfNotFilledCellExists() const {
        return std::any_of(grid_.begin(), grid_.end(), [](const auto& cell) { return cell.getNumber() == 0; });
    }

    Cell& LatinSquare::getNotFilledCellWithMinimumEntropy() { // TODO: should return ref to cell from grid
        std::sort(grid_.begin(), grid_.end(),
            [](const auto& firstCell, const auto& secondCell) {
                return firstCell.getEntropy() < secondCell.getEntropy();
            });
        const auto& iterator = std::find_if(
            grid_.begin(), grid_.end(), [](const auto& cell) { return cell.getNumber() == 0; });

        return *iterator;
    }

    const FilledCellData LatinSquare::getFilledCellData(
        const std::string& id, const int number, const EntropyData& previousEntropyData) const {
        return FilledCellData(id, number, previousEntropyData);
    }

    bool LatinSquare::checkCellRow(const Cell& cell, const int row) const {
        return cell.getRow() == row;
    }

    bool LatinSquare::checkCellColumn(const Cell& cell, const int column) const {
        return cell.getColumn() == column;
    }

    const std::set<std::string> LatinSquare::getUpdatedCellsIds(
        const int row, const int column, const int number) const { // TODO: investigate if function works correctly
        std::vector<Cell> relatedCells;
        std::copy_if(
            grid_.begin(), grid_.end(), std::back_inserter(relatedCells),
            [this, row, column](const auto& cell) { return checkCellRow(cell, row) != checkCellColumn(cell, column); });

        std::set<std::string> updatedCellsIds;

        for (auto& cell : relatedCells) {
            if (cell.removeRemainingNumber(number)) {
                updatedCellsIds.insert(cell.getId());
            }
        }

        return updatedCellsIds;
    }

    const UpdateData LatinSquare::getUpdateData(
        const std::string& id, const int row, const int column, const int number,
        const EntropyData& previousEntropyData) const {
        return UpdateData(getFilledCellData(id, number, previousEntropyData), getUpdatedCellsIds(row, column, number));
    }

    const std::vector<Cell> LatinSquare::getPreviousUpdatedCells(const std::set<std::string>& updatedCellsIds) const { // TODO: should return vector of refs to cells from grid
        std::vector<Cell> previousUpdatedCells;
        std::copy_if(
            grid_.begin(), grid_.end(), std::back_inserter(previousUpdatedCells),
            [this, &updatedCellsIds](const auto& cell) { return updatedCellsIds.contains(cell.getId()); });

        return previousUpdatedCells;
    }
}
