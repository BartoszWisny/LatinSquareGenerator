#pragma once

#include <optional>
#include <set>
#include <vector>

#include "LatinSquareCell.hpp"
#include "LatinSquareUtils.hpp"

namespace LatinSquareGenerator {
    class LatinSquare {
        public:
            LatinSquare(int size);

            void setSize(int size);

            std::vector<Cell> getGrid();

            bool checkCellRow(Cell cell, int row);
            bool checkCellColumn(Cell cell, int column);

            Cell getCell(Position position);

            int calculateCellRow(int index);
            int calculateCellColumn(int index);

            void reset();

            bool checkIfNotFilledCellExists();
            Cell getNotFilledCellWithMinimumEntropy();

            FilledCellData getFilledCellData(Position position, int number, EntropyData previousEntropyData);
            std::vector<Position> getUpdatedCells(Position position, int number);
            UpdateData getUpdateData(Position position, int number, EntropyData previousEntropyData);

            // std::vector<Cell> getPreviousUpdatedCells();

        private:
            int size;
            std::vector<Cell> grid;

            int gridSize;
    };
}
