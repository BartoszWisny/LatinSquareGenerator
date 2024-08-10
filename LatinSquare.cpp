#include "LatinSquare.hpp"

#include <algorithm>

namespace LatinSquareGenerator {
    LatinSquare::LatinSquare(int size) {
        setSize(size);
        reset();
    }

    void LatinSquare::setSize(int size) {
        size = size;
        gridSize = size * size;
    }

    std::vector<Cell> LatinSquare::getGrid() {
        return grid;
    }

    int LatinSquare::calculateCellRow(int index) {
        return index / size;
    }

    int LatinSquare::calculateCellColumn(int index) {
        return index % size;
    }

    void LatinSquare::reset() {
        grid = std::vector<Cell>();

        for (int index = 0; index < gridSize; index++) {
            grid.push_back(Cell(calculateCellRow(index), calculateCellColumn(index), size));
        }
    }

    bool LatinSquare::checkCellRow(Cell cell, int row) {
        return cell.getRow() == row;
    }

    bool LatinSquare::checkCellColumn(Cell cell, int column) {
        return cell.getColumn() == column;
    }

    bool LatinSquare::checkAllCellsFilled() {
        return std::all_of(grid.begin(), grid.end(), [](Cell cell) { return cell.getNumber() != 0; });
    }

    std::optional<Cell> LatinSquare::getCellWithMinimumEntropy() {
        std::sort(grid.begin(), grid.end(),
                  [](Cell firstCell, Cell secondCell) { return firstCell.getEntropy() < secondCell.getEntropy(); });

        std::vector<Cell> cellsWithNonZeroEntropy;
        std::copy_if(grid.begin(), grid.end(), std::back_inserter(cellsWithNonZeroEntropy),
                     [](Cell cell) { return cell.getEntropy() != 0; });

        std::optional<Cell> maybeCellWithNonZeroEntropy;

        if (!cellsWithNonZeroEntropy.empty()) {
            maybeCellWithNonZeroEntropy = cellsWithNonZeroEntropy.front();
        }

        return maybeCellWithNonZeroEntropy;
    }

    // This function must be used before filling cell because previous entropy data are needed for update history in case of reverting filling this cell.
    FilledCellData LatinSquare::getFilledCellData(Position position, int number, EntropyData previousEntropyData) {
        return FilledCellData(position, number, previousEntropyData);
    }

    std::set<Position> LatinSquare::getUpdatedCells(Position position, int number) {
        int row = position.getRow();
        int column = position.getColumn();

        std::vector<Cell> relatedCells;
        std::copy_if(grid.begin(), grid.end(), std::back_inserter(relatedCells),
                     [this, row, column](Cell cell) {
                         return checkCellRow(cell, row) != checkCellColumn(cell, column);
                     });

        std::set<Position> updatedCells;

        for (Cell cell : relatedCells) {
            if (cell.removeRemainingNumber(number)) {
                updatedCells.insert(cell.getPosition());
            }
        }

        return updatedCells;
    }

    UpdateData LatinSquare::getUpdateData(Position position, int number, EntropyData previousEntropyData) {
        return UpdateData(getFilledCellData(position, number, previousEntropyData), getUpdatedCells(position, number));
    }
}
