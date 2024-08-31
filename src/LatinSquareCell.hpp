#pragma once

#include <set>
#include <string>

#include "LatinSquareEntropyData.hpp"

namespace LatinSquareGenerator {
    class Cell {
        public:
            Cell(const int row, const int column, const int maxEntropy);

            int getRow() const;
            int getColumn() const;
            int getNumber() const;
            const std::string& getId() const;
            const std::string& getFullId() const;
            int getEntropy() const;
            const std::set<int>& getRemainingNumbers() const;
            const EntropyData& getEntropyData() const;

            bool isAvailable() const;
            void setAvailable(const bool available);

            void reset();
            void fill(const int number);
            void clear(EntropyData previousEntropyData);

            bool removeRemainingNumber(const int number);
            void restoreRemainingNumber(const int number);

        private:
            void setRow(const int row);
            void setColumn(const int column);
            void setNumber(const int number);
            void setId();
            void setFullId();
            void setEntropyData(const EntropyData& entropyData);

            void setMaxEntropy(const int maxEntropy);

            int row_;
            int column_;
            int number_;
            std::string id_;
            std::string fullId_;
            EntropyData entropyData_;

            bool available_;
    };
}
