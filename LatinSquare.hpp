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

            int calculateCellRow(int index);
            int calculateCellColumn(int index);

            void reset();

            bool checkCellRow(Cell cell, int row);
            bool checkCellColumn(Cell cell, int column);

            bool checkAllCellsFilled();
            // TODO: Goal is to have function that finds not filled cell with zero entropy.
            std::optional<Cell> getCellWithMinimumEntropy(); // This function will not find not filled cell with zero entropy.
            // So also add new function for this case.

            FilledCellData getFilledCellData(Position position, int number, EntropyData previousEntropyData);
            std::set<Position> getUpdatedCells(Position position, int number);
            UpdateData getUpdateData(Position position, int number, EntropyData previousEntropyData);

        private:
            int size;
            std::vector<Cell> grid;

            int gridSize;
    };
}
