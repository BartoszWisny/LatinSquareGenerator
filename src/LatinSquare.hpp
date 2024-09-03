#pragma once

#include <functional>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "LatinSquareCell.hpp"
#include "LatinSquareEntropyData.hpp"
#include "LatinSquareRegions.hpp"
#include "LatinSquareUpdateData.hpp"

namespace LatinSquareGenerator {
    class LatinSquare {
        public:
            LatinSquare();
            LatinSquare(const int size, const std::mt19937& mersenneTwister);

            int getSize() const;

            void sortGridByRows();
            void sortGridByColumns();
            void sortGridByNumbers();

            const std::vector<Cell>& getGrid();

            int getGridSize() const;

            void reset();

            Cell& getCell(const std::string& id);

            bool checkIfNotFilledCellExists() const;
            Cell& getNotFilledCellWithMinimumEntropy();
            const UpdateData getUpdateData(const Cell& filledCell, const EntropyData& previousEntropyData);
            const std::vector<std::reference_wrapper<Cell>> getPreviousUpdatedCells(
                const std::set<std::string>& updatedCellsIds);

            const std::set<std::string> disableRelatedCells(const Cell& chosenCell, LatinSquareRegions& regions);

        private:
            void setSize(const int size);

            void setMersenneTwister(const std::mt19937& mersenneTwister);

            bool checkIfRelatedToFilledCell(const Cell& filledCell, const Cell& cell) const;
            const std::set<std::string> getUpdatedCellsIds(const Cell& filledCell);

            bool checkIfRelatedToChosenCell(const Cell& chosenCell, const Cell& cell) const;

            int size_;
            std::vector<Cell> grid_;

            int gridSize_;
            std::mt19937 mersenneTwister_;
    };
}
