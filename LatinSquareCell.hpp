#pragma once

#include <set>

namespace LatinSquareGenerator {
    class LatinSquareCell {
        public:
            LatinSquareCell(int latinSquareSize);

            int getRow();
            void setRow(int row);

            int getColumn();
            void setColumn(int column);

            void setPosition(int row, int column);

            int getMaxEntropy();
            void setMaxEntropy(int maxEntropy);

            int getNumber();
            void setNumber(int number);
            void resetNumber();

            int getEntropy();
            void setEntropy(int entropy);
            void resetEntropy();
            void clearEntropy();

            std::set<int> getRemainingNumbers();
            void setRemainingNumbers(std::set<int> remainingNumbers);
            void resetRemainingNumbers();
            void clearRemainingNumbers();

            void setData(int number, int entropy, std::set<int> remainingNumbers);
            void resetData();

            void clearEntropyData();

            int getPreviousEntropy();
            void setPreviousEntropy(int previousEntropy);
            void resetPreviousEntropy();

            std::set<int> getPreviousRemainingNumbers();
            void setPreviousRemainingNumbers(std::set<int> previousRemainingNumbers);
            void resetPreviousRemainingNumbers();

            void setPreviousEntropyData(int previousEntropy, std::set<int> previousRemainingNumbers);
            void resetPreviousEntropyData();

            void rememberPreviousEntropyData();
            void restorePreviousEntropyData();

            void fill(int number);
            void clear();

            // void removeRemainingNumber(int number);
            // void restoreRemainingNumber(int number);

        private:
            int row;
            int column;
            int maxEntropy;

            int number;
            int entropy;
            std::set<int> remainingNumbers;

            int previousEntropy;
            std::set<int> previousRemainingNumbers;
    };
}
