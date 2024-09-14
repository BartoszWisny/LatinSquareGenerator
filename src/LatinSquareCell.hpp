#pragma once

#include <set>
#include <string>

#include "LatinSquareEntropyData.hpp"

namespace LatinSquareGenerator {
    class Cell {
        public:
            Cell(const int row, const int column, const bool reduced, const int maxEntropy);

            int getRow() const;
            int getColumn() const;
            int getNumber() const;
            const std::string& getRowId() const;
            const std::string& getColumnId() const;
            const std::string& getId() const;
            const std::string& getNumberId() const;
            const std::string& getFullId() const;
            int getEntropy() const;
            const std::set<int>& getRemainingNumbers() const;
            const EntropyData& getEntropyData() const;
            void setEntropyData(const EntropyData& entropyData);
            bool isEnabled() const;
            void enable();
            void disable();

            void reset(const bool reduced);
            void fill(const int number);
            void clear(EntropyData entropyData);

            bool removeRemainingNumber(const int number);
            void restoreRemainingNumber(const int number);

        private:
            void setRow(const int row);
            void setColumn(const int column);
            void setNumber(const int number);
            void setIds();
            void setFullId();

            void setMaxEntropy(const int maxEntropy);

            int row_;
            int column_;
            int number_;
            std::string rowId_;
            std::string columnId_;
            std::string id_;
            std::string numberId_;
            std::string fullId_;
            EntropyData entropyData_;
            bool enabled_;
    };
}
