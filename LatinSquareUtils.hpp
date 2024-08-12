#pragma once

#include <set>
#include <vector>

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

            std::set<int> getRemainingNumbers();
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

            Position getPosition();
            void setPosition(Position position);

            int getNumber();
            void setNumber(int number);

            EntropyData getPreviousEntropyData();
            void setPreviousEntropyData(EntropyData previousEntropyData);

        private:
            Position position;
            int number;
            EntropyData previousEntropyData;
    };

    class UpdateData {
        public:
            UpdateData();
            // TODO: check if using vector of refs to cells will be better idea
            UpdateData(FilledCellData filledCellData, std::vector<Position> updatedCells);

            FilledCellData getFilledCellData();
            void setFilledCellData(FilledCellData filledCellData);

            std::vector<Position> getUpdatedCells();
            void setUpdatedCells(std::vector<Position> updatedCells);

        private:
            FilledCellData filledCellData;
            std::vector<Position> updatedCells;
    };
}
