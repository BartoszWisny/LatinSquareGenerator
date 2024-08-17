#pragma once

#include <set>
#include <string>

#include "LatinSquareUtils.hpp"

namespace LatinSquareGenerator {
    class Cell {
        public:
            Cell(int row, int column, int maxEntropy);

            int getRow() const;

            int getColumn() const;

            std::string getId() const;

            int getNumber() const;

            std::string getFullId() const;

            EntropyData getEntropyData();

            int getEntropy() const;

            std::set<int> getRemainingNumbers();

            void fill(int number);
            void clear(EntropyData previousEntropyData);

            bool removeRemainingNumber(int number);
            void restoreRemainingNumber(int number);

        private:
            void setRow(int row);

            void setColumn(int column);

            void setId();

            void setNumber(int number);
            void resetNumber();

            void setFullId();
            void resetFullId();

            void setEntropyData(EntropyData entropyData);
            void resetEntropyData();
            void clearEntropyData();

            void setMaxEntropy(int maxEntropy);

            void reset();

            int row_;
            int column_;
            std::string id_;
            int number_;
            std::string fullId_;
            EntropyData entropyData_;
    };
}
