#pragma once

#include <functional>
#include <random>
#include <string>
#include <vector>

#include "Cell.hpp"
#include "Region.hpp"
#include "Types.hpp"

namespace LatinSquare {
    class LatinSquare {
        public:
            LatinSquare();
            LatinSquare(const int size, const Type type);
            LatinSquare(const int size, const Type type, std::mt19937& mersenneTwister);

            int getSize() const;
            const std::vector<Cell>& getGrid() const;
            void setRegions();

            void sortGrid();

            Cell& getCell(const std::string& id);
            const std::vector<std::reference_wrapper<Cell>> getCells(const std::vector<std::string>& ids);
            const std::vector<std::reference_wrapper<Region>> getRegions(const std::vector<std::string>& ids);

            bool checkIfNotFilledCellExists() const;
            Cell& getNotFilledCellWithMinimumEntropy();
            Cell& getRandomNotFilledCellWithMinimumEntropy();
            const std::vector<std::reference_wrapper<Cell>> getCellsRelatedToFilledCell(const Cell& filledCell);
            void updateRelatedCells(std::vector<std::reference_wrapper<Cell>>& cells, const int number);
            const std::vector<std::string> getUpdatedCellsIds(const std::vector<std::reference_wrapper<Cell>>& cells);
            void restoreUpdatedCells(const std::vector<std::reference_wrapper<Cell>>& cells, const int number);

            Region& getEnabledRegionWithMinimumEntropy();
            Region& getRandomEnabledRegionWithMinimumEntropy();
            const std::vector<std::reference_wrapper<Cell>> getCellsRelatedToChosenCell(const Cell& chosenCell);
            void disableRelatedCells(const std::vector<std::reference_wrapper<Cell>>& cells);
            void decreaseRelatedRegionsEntropy(const std::vector<std::reference_wrapper<Cell>>& cells);
            const std::vector<std::string> getDisabledCellsIds(
                const std::vector<std::reference_wrapper<Cell>>& cells, const Cell& chosenCell);
            const std::vector<std::reference_wrapper<Region>> getRelatedRegions(const Cell& cell);
            void disableRelatedRegions(const std::vector<std::reference_wrapper<Region>>& regions);
            void enableDisabledCells(const std::vector<std::reference_wrapper<Cell>>& cells);
            void increaseRelatedRegionsEntropy(const std::vector<std::reference_wrapper<Cell>>& cells);
            void enableRelatedRegions(const std::vector<std::reference_wrapper<Region>>& regions);

        private:
            void setGrid(const Type type);

            bool checkIfNotFilledAndRelatedToFilledCell(const Cell& filledCell, const Cell& cell) const;

            bool checkIfEnabledAndRelatedToChosenCell(const Cell& chosenCell, const Cell& cell) const;
            bool checkIfRelatedRegion(const Cell& cell, const Region& region) const;

            int size_;
            // consider using std::unique_ptr for Cell and Region
            std::vector<Cell> grid_;
            std::vector<Region> regions_;
            std::mt19937 mersenneTwister_;
    };
}
