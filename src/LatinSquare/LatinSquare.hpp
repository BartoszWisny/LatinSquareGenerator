#pragma once

#include <functional>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "Cell.hpp"
#include "Region.hpp"

namespace LatinSquare {
    class LatinSquare {
        public:
            LatinSquare();
            LatinSquare(const int size, const bool reduced, const std::mt19937& mersenneTwister);

            int getSize() const;
            const std::vector<Cell>& getGrid() const;
            void setGrid(const bool reduced);
            const std::vector<Region>& getRegions() const;
            void setRegions();

            void sortGrid();
            void shuffleGrid();

            Cell& getCell(const std::string& id);
            const std::vector<std::reference_wrapper<Cell>> getCells(const std::set<std::string>& ids);
            const std::vector<std::reference_wrapper<Region>> getRegions(const std::set<std::string>& ids);

            bool checkIfNotFilledCellExists() const;
            Cell& getNotFilledCellWithMinimumEntropy();
            const std::vector<std::reference_wrapper<Cell>> getCellsRelatedToFilledCell(const Cell& filledCell);
            void updateRelatedCells(std::vector<std::reference_wrapper<Cell>>& cells, const int number);
            const std::set<std::string> getUpdatedCellsIds(const std::vector<std::reference_wrapper<Cell>>& cells);
            void restoreUpdatedCells(const std::vector<std::reference_wrapper<Cell>>& cells, const int number);

            Region& getEnabledRegionWithMinimumEntropy();
            const std::vector<std::reference_wrapper<Cell>> getCellsRelatedToChosenCell(const Cell& chosenCell);
            void disableRelatedCells(const std::vector<std::reference_wrapper<Cell>>& cells);
            void decreaseRelatedRegionsEntropy(const std::vector<std::reference_wrapper<Cell>>& cells);
            const std::set<std::string> getDisabledCellsIds(
                const std::vector<std::reference_wrapper<Cell>>& cells, const Cell& cell);
            const std::vector<std::reference_wrapper<Region>> getRelatedRegions(const Cell& cell);
            void disableRelatedRegions(const std::vector<std::reference_wrapper<Region>>& regions);
            void enableDisabledCells(const std::vector<std::reference_wrapper<Cell>>& cells);
            void increaseRelatedRegionsEntropy(const std::vector<std::reference_wrapper<Cell>>& cells);
            void enableRelatedRegions(const std::vector<std::reference_wrapper<Region>>& regions);

        private:
            bool checkIfRelatedToFilledCell(const Cell& filledCell, const Cell& cell) const;

            bool checkIfRelatedToChosenCell(const Cell& chosenCell, const Cell& cell) const;
            bool checkIfRelatedRegion(const Cell& cell, const Region& region) const;

            int size_;
            // consider using std::unique_ptr for Cell and Region
            std::vector<Cell> grid_;
            std::vector<Region> regions_;
            std::mt19937 mersenneTwister_;
    };
}
