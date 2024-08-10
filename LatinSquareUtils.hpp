#pragma once

#include <set>

namespace LatinSquareGenerator {
    class Position {
        public:
            Position();
            Position(int row, int column);

            int getRow();
            void setRow(int row);

            int getColumn();
            void setColumn(int column);

        private:
            int row;
            int column;
    };

    class EntropyData {
        public:
            EntropyData();
            EntropyData(int entropy, std::set<int> remainingNumbers);

            int getEntropy();
            void setEntropy(int entropy);
            void resetEntropy();
            void clearEntropy();

            void setRemainingNumbers(std::set<int> remainingNumbers);
            void resetRemainingNumbers();
            void clearRemainingNumbers();

            void setMaxEntropy(int maxEntropy);

            bool removeRemainingNumber(int number);
            void restoreRemainingNumber(int number);

        private:
            int entropy;
            std::set<int> remainingNumbers;

            int maxEntropy;
    };

    class FilledCellData {
        public:
            FilledCellData();
            FilledCellData(Position position, int number, EntropyData previousEntropyData);

            void setPosition(Position position);
            void setNumber(int number);
            void setPreviousEntropyData(EntropyData previousEntropyData);

        private:
            Position position;
            int number;
            EntropyData previousEntropyData;
    };

    class UpdateData {
        public:
            UpdateData();
            UpdateData(FilledCellData filledCellData, std::set<Position> updatedCells);

            void setFilledCellData(FilledCellData filledCellData);
            void setUpdatedCells(std::set<Position> updatedCells);

        private:
            FilledCellData filledCellData;
            std::set<Position> updatedCells;
    };
}
