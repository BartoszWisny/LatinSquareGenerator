#pragma once

#include <functional>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "LatinSquareCell.hpp"
#include "LatinSquareEntropyData.hpp"
#include "LatinSquareRegion.hpp"
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
            const std::vector<Region>& getRegions() const;

            void reset();

            void shuffleGrid();

            Cell& getCell(const std::string& id);
            const std::vector<std::reference_wrapper<Cell>> getCells(const std::set<std::string>& ids);
            const std::vector<std::reference_wrapper<Region>> getRegions(const std::set<std::string>& ids);

            bool checkIfNotFilledCellExists() const;
            Cell& getNotFilledCellWithMinimumEntropy();
            const UpdateData getUpdateData(const Cell& filledCell, const EntropyData& previousEntropyData);

            // check these functions
            Region& getEnabledRegionWithMinimumEntropy();
            const std::set<std::string> getDisabledCellsIds(const Cell& chosenCell);
            void disableRelatedRegions(const Cell& cell);

        private:
            void setSize(const int size);
            void setRegions();

            void setMersenneTwister(const std::mt19937& mersenneTwister);

            bool checkIfRelatedToFilledCell(const Cell& filledCell, const Cell& cell) const;
            const std::set<std::string> getUpdatedCellsIds(const Cell& filledCell);

            // check these functions
            bool checkIfRelatedToChosenCell(const Cell& chosenCell, const Cell& cell) const;
            void decreaseRelatedRegionsEntropy(
                const std::set<std::string>& relatedRegionsIds, const std::map<std::string, int>& entropyUpdates);
            bool checkIfRelatedRegion(const Cell& cell, const Region& region) const;

            int size_;
            std::vector<Cell> grid_;
            std::vector<Region> regions_;

            int gridSize_;
            std::mt19937 mersenneTwister_;
    };
}
