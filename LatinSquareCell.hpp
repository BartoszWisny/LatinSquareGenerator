#pragma once

#include <set>

#include "LatinSquareUtils.hpp"

namespace LatinSquareGenerator {
    class Cell {
        public:
            Cell(int row, int column, int maxEntropy);

            Position getPosition();
            void setPosition(int row, int column);

            int getRow();
            int getColumn();

            int getNumber();
            void setNumber(int number);
            void resetNumber();

            EntropyData getEntropyData();
            void setEntropyData(EntropyData entropyData);
            void resetEntropyData();
            void clearEntropyData();

            int getEntropy();
            std::set<int> getRemainingNumbers();

            void setMaxEntropy(int maxEntropy);

            void reset();
            void fill(int number);
            void clear(EntropyData previousEntropyData);

            bool removeRemainingNumber(int number);
            void restoreRemainingNumber(int number);

        private:
            Position position;
            int maxEntropy;

            int number;
            EntropyData entropyData;

            int previousEntropy;
            std::set<int> previousRemainingNumbers;
    };
}
