#pragma once

#include <set>
#include <string>
#include <vector>

#include "LatinSquareCell.hpp"
#include "LatinSquareUtils.hpp"

namespace LatinSquareGenerator {
    class LatinSquare {
        public:
            LatinSquare(const int size);

            const std::vector<Cell>& getGrid();

            Cell& getCell(const std::string& id);

            void reset();

            bool checkIfNotFilledCellExists() const;
            Cell& getNotFilledCellWithMinimumEntropy();

            const FilledCellData getFilledCellData(
                const std::string& id, const int number, const EntropyData& previousEntropyData) const;
            const std::set<std::string> getUpdatedCellsIds(const int row, const int column, const int number) const;
            const UpdateData getUpdateData(
                const std::string& id, const int row, const int column, const int number,
                const EntropyData& previousEntropyData) const;

            const std::vector<Cell> getPreviousUpdatedCells(const std::set<std::string>& updatedCellsIds) const;

        private:
            void setSize(const int size);

            int calculateCellRow(const int index) const;
            int calculateCellColumn(const int index) const;

            bool checkCellRow(const Cell& cell, const int row) const;
            bool checkCellColumn(const Cell& cell, const int column) const;

            int size_;
            std::vector<Cell> grid_;

            int gridSize_;
    };
}
