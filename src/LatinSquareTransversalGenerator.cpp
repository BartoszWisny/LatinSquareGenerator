#include "LatinSquareTransversalGenerator.hpp"

#include <iterator>
#include <random>

namespace LatinSquareGenerator {
    bool LatinSquareTransversalGenerator::checkIfAddToBacktrackingHistory(const Cell& cell) const {
        return backtrackingHistory_.empty() || cell.getId() != backtrackingHistory_.top().getChosenCellId();
    }

    bool LatinSquareTransversalGenerator::checkIfRemoveFromBacktrackingHistory(const std::string& regionId) const {
        return !backtrackingHistory_.empty() && backtrackingHistory_.top().getRegionId() == regionId;
    }

    const std::vector<std::reference_wrapper<Cell>> LatinSquareTransversalGenerator::findRandomTransversal(
        LatinSquare& latinSquare) {
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());
        std::vector<std::reference_wrapper<Cell>> transversal;
        const auto size = latinSquare.getSize();
        int counter = 0;

        while (transversal.size() < (unsigned) size) {
            auto& region = latinSquare.getEnabledRegionWithMinimumEntropy();

            if (region.getEntropy() > 0) {
                counter = 0;

                const auto cells = region.getEnabledCells();
                auto iterator = cells.cbegin();
                std::advance(iterator, mersenneTwister() % cells.size());
                auto& cell = (*iterator).get();

                transversal.emplace_back(*iterator);
                const auto disabledCellsIds = latinSquare.getDisabledCellsIds(cell);
                latinSquare.disableRelatedRegions(cell);
                updateHistory_.push(TransversalUpdateData(cell, disabledCellsIds));

                if (checkIfAddToBacktrackingHistory(cell)) {
                    backtrackingHistory_.push(TransversalBacktrackingData(region, cell));
                }
            } else {
                ++counter;

                if (counter > 1) {
                    auto backtrackingData = backtrackingHistory_.top();
                    backtrackingHistory_.pop();
                    std::set<std::string> disabledCellsIds = {backtrackingData.getChosenCellId()};
                    const std::string regionId = backtrackingData.getRegionId();

                    while (checkIfRemoveFromBacktrackingHistory(regionId)) {
                        backtrackingData = backtrackingHistory_.top();
                        backtrackingHistory_.pop();
                        disabledCellsIds.insert(backtrackingData.getChosenCellId());
                    }

                    latinSquare.enableCells(disabledCellsIds);

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                transversal.pop_back();
                const auto updateData = updateHistory_.top();
                updateHistory_.pop();

                latinSquare.enableCells(updateData.getDisabledCellsIds());
                latinSquare.enableRelatedRegions(latinSquare.getCell(updateData.getChosenCellId()));
            }
        }

        return transversal;
    }
}
