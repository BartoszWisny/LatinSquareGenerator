#pragma once

#include <vector>

namespace LatinSquareGenerator {
    class LatinSquareCell {
        public:
            LatinSquareCell(int latinSquareSize);

            int getRow();
            void setRow(int row);

            int getColumn();
            void setColumn(int column);

            void setPosition(int row, int column);

            int getNumber();
            void setNumber(int number);
            void resetNumber();

            int getEntropy();
            void setEntropy(int entropy);
            void resetEntropy();

            std::vector<int> getRemainingNumbers();
            void setRemainingNumbers(std::vector<int> remainingNumbers);
            void resetRemainingNumbers();

            void setData(int number, int entropy, std::vector<int> remainingNumbers);
            void resetData();

            int getLatinSquareSize();
            void setLatinSquareSize(int latinSquareSize);

            // void fill(int number);
            // void removeRemainingNumber(int number);

        private:
            int row;
            int column;
            int number;
            int entropy;
            std::vector<int> remainingNumbers;
            int latinSquareSize;
    };
}
