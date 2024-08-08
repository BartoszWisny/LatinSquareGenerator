#include "LatinSquare.hpp"

namespace LatinSquareGenerator {
    LatinSquare::LatinSquare(int size) {
        setSize(size);
        resetGrid();
    }

    int LatinSquare::getSize() {
        return size;
    }

    void LatinSquare::setSize(int size) {
        size = size;
    }

    std::vector<LatinSquareCell> LatinSquare::getGrid() {
        return grid;
    }

    void LatinSquare::setGrid(std::vector<LatinSquareCell> grid) {
        grid = grid;
    }

    void LatinSquare::resetGrid() {
        setGrid(std::vector<LatinSquareCell>(size * size, LatinSquareCell(size)));

        for (int gridIndex = 0; gridIndex < grid.size(); gridIndex++) {
            setCellPosition(gridIndex);
        }
    }

    int LatinSquare::getGridIndex(int row, int column) {
        return row * getSize() + column;
    }

    LatinSquareCell LatinSquare::getCell(int row, int column) {
        return grid.at(getGridIndex(row, column));
    }

    int LatinSquare::getCellRow(int gridIndex) {
        return gridIndex / getSize();
    }

    void LatinSquare::setCellRow(int gridIndex) {
        int row = getCellRow(gridIndex);

        getCell(row, getCellColumn(gridIndex)).setRow(row);
    }

    int LatinSquare::getCellColumn(int gridIndex) {
        return gridIndex % getSize();
    }

    void LatinSquare::setCellColumn(int gridIndex) {
        int column = getCellColumn(gridIndex);

        getCell(getCellRow(gridIndex), column).setColumn(column);
    }

    void LatinSquare::setCellPosition(int gridIndex) {
        int row = getCellRow(gridIndex);
        int column = getCellColumn(gridIndex);

        getCell(row, column).setPosition(row, column);
    }

    int LatinSquare::getCellNumber(int row, int column) {
        return getCell(row, column).getNumber();
    }

    void LatinSquare::setCellNumber(int row, int column, int number) {
        getCell(row, column).setNumber(number);
    }

    void LatinSquare::resetCellNumber(int row, int column) {
        getCell(row, column).resetNumber();
    }

    int LatinSquare::getCellEntropy(int row, int column) {
        return getCell(row, column).getEntropy();
    }

    void LatinSquare::setCellEntropy(int row, int column, int entropy) {
        getCell(row, column).setEntropy(entropy);
    }

    void LatinSquare::resetCellEntropy(int row, int column) {
        getCell(row, column).resetEntropy();
    }

    std::set<int> LatinSquare::getCellRemainingNumbers(int row, int column) {
        getCell(row, column).getRemainingNumbers();
    }

    void LatinSquare::setCellRemainingNumbers(int row, int column, std::set<int> remainingNumbers) {
        getCell(row, column).setRemainingNumbers(remainingNumbers);
    }

    void LatinSquare::resetCellRemainingNumbers(int row, int column) {
        getCell(row, column).resetRemainingNumbers();
    }

    void LatinSquare::setCellData(int row, int column, int number, int entropy, std::set<int> remainingNumbers) {
        getCell(row, column).setData(number, entropy, remainingNumbers);
    }

    void LatinSquare::resetCellData(int row, int column) {
        getCell(row, column).resetData();
    }

    // void LatinSquare::fillCell(int row, int column, int number) {
    //     getCell(row, column).fill(number);
    // }

    // void LatinSquare::removeCellRemainingNumber(int row, int column, int number) {
    //     getCell(row, column).removeRemainingNumber(number);
    // }
}
