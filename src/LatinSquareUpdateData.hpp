#pragma once

#include <set>
#include <string>

#include "LatinSquareCell.hpp"
#include "LatinSquareEntropyData.hpp"

namespace LatinSquareGenerator {
    class UpdateData {
        public:
            UpdateData(const Cell& filledCell, const EntropyData& previousEntropyData,
                       const std::set<std::string>& updatedCellsIds);

            const std::string& getFilledCellId() const;
            int getFilledCellNumber() const;
            const EntropyData& getPreviousEntropyData() const;
            const std::set<std::string>& getUpdatedCellsIds() const;

        private:
            void setFilledCellId(const std::string& filledCellId);
            void setFilledCellNumber(const int number);
            void setPreviousEntropyData(const EntropyData& previousEntropyData);
            void setUpdatedCellsIds(const std::set<std::string>& updatedCellsIds);

            std::string filledCellId_;
            int filledCellNumber_;
            EntropyData previousEntropyData_;
            std::set<std::string> updatedCellsIds_;
    };
}
