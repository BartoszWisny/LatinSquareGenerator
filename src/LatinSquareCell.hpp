#pragma once

#include <set>
#include <string>

#include "LatinSquareUtils.hpp"

namespace LatinSquareGenerator {
    class Cell {
        public:
            Cell(const int row, const int column, const int maxEntropy);

            int getRow() const;

            int getColumn() const;

            const std::string& getId() const;

            int getNumber() const;

            const std::string& getFullId() const;

            const EntropyData& getEntropyData() const;

            int getEntropy() const;

            const std::set<int>& getRemainingNumbers() const;

            void fill(const int number);
            void clear(EntropyData previousEntropyData);

            bool removeRemainingNumber(const int number);
            void restoreRemainingNumber(const int number);

        private:
            void setRow(const int row);

            void setColumn(const int column);

            void setId();

            void setNumber(const int number);
            void resetNumber();

            void setFullId();

            void setEntropyData(const EntropyData& entropyData);
            void resetEntropyData();
            void clearEntropyData();

            void setMaxEntropy(const int maxEntropy);

            void reset();

            int row_;
            int column_;
            std::string id_;
            int number_;
            std::string fullId_;
            EntropyData entropyData_;
    };
}
