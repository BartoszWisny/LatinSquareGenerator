#pragma once

#include <functional>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "LatinSquareCell.hpp"
#include "LatinSquareEntropyData.hpp"
#include "LatinSquareUpdateData.hpp"

namespace LatinSquareGenerator {
    class LatinSquare {
        public:
            LatinSquare(const int size, const std::mt19937& mersenneTwister);

            int getSize() const;
            const std::vector<Cell>& getGrid();

            int getGridSize() const;

            void reset();

            Cell& getCell(const std::string& id);

            bool checkIfNotFilledCellExists() const;
            Cell& getNotFilledCellWithMinimumEntropy();
            const UpdateData getUpdateData(const Cell& filledCell, const EntropyData& previousEntropyData);
            const std::vector<std::reference_wrapper<Cell>> getPreviousUpdatedCells(
                const std::set<std::string>& updatedCellsIds);

        private:
            void setSize(const int size);

            void setMersenneTwister(const std::mt19937& mersenneTwister);

            bool checkIfRelatedCell(const Cell& filledCell, const Cell& cell) const;
            const std::set<std::string> getUpdatedCellsIds(const Cell& filledCell);

            int size_;
            std::vector<Cell> grid_;

            int gridSize_;
            std::mt19937 mersenneTwister_;
    };
}
