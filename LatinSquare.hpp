#pragma once

#include <vector>

#include "LatinSquareCell.hpp"

namespace LatinSquareGenerator {
    class LatinSquare {
        public:
            LatinSquare(int size);

            int getSize();
            void setSize(int size);

            std::vector<LatinSquareCell> getGrid();
            void setGrid(std::vector<LatinSquareCell> grid);
            void resetGrid();

            int getGridIndex(int row, int column);

            LatinSquareCell getCell(int row, int column);

            int getCellRow(int gridIndex);
            void setCellRow(int gridIndex);

            int getCellColumn(int gridIndex);
            void setCellColumn(int gridIndex);

            void setCellPosition(int gridIndex);

            int getCellNumber(int row, int column);
            void setCellNumber(int row, int column, int number);
            void resetCellNumber(int row, int column);

            int getCellEntropy(int row, int column);
            void setCellEntropy(int row, int column, int entropy);
            void resetCellEntropy(int row, int column);

            std::vector<int> getCellRemainingNumbers(int row, int column);
            void setCellRemainingNumbers(int row, int column, std::vector<int> remainingNumbers);
            void resetCellRemainingNumbers(int row, int column);

            void setCellData(int row, int column, int number, int entropy, std::vector<int> remainingNumbers);
            void resetCellData(int row, int column);

            // void fillCell(int row, int column, int number);
            // void removeCellRemainingNumber(int row, int column, int number);

        private:
            int size;
            std::vector<LatinSquareCell> grid;
    };
}
