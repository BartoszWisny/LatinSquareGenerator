#pragma once

#include <string>
#include <vector>

#include "EntropyData.hpp"

namespace LatinSquare {
    class Cell {
        public:
            Cell(const int row, const int column, const bool reduced, const int latinSquareSize);

            int getRow() const;
            int getColumn() const;
            int getNumber() const;
            const std::string& getRowId() const;
            const std::string& getColumnId() const;
            const std::string& getId() const;
            const std::string& getNumberId() const;
            const std::string& getFullId() const;
            int getRowIdAsInt() const;
            int getColumnIdAsInt() const;
            int getNumberIdAsInt() const;
            int getEntropy() const;
            const std::vector<int>& getRemainingNumbers() const;
            const EntropyData& getEntropyData() const;
            void setEntropyData(const EntropyData& entropyData);
            bool isFilled() const;
            void setFilled();
            void setNotFilled();
            bool isEnabled() const;
            void enable();
            void disable();

            void reset(const bool reduced);
            void fill(const int number);
            void clear(EntropyData entropyData);

            bool removeRemainingNumber(const int number);
            void restoreRemainingNumber(const int number);

        private:
            void setNumber(const int number);
            void setIds();
            void setFullId();
            void setNumberIdAsInt();

            int row_, column_, number_, latinSquareSize_, rowIdAsInt_, columnIdAsInt_, numberIdAsInt_;
            std::string rowId_, columnId_, id_, numberId_, fullId_;
            EntropyData entropyData_;
            bool filled_, enabled_;
    };
}
