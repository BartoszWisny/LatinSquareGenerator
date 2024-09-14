#include "Generator.hpp"

#include <iterator>
#include <random>
#include <set>

namespace Transversal {
    bool Generator::checkIfAddToBacktrackingHistory(const LatinSquare::Cell& cell) const {
        return backtrackingHistory_.empty() || cell.getId() != backtrackingHistory_.top().getChosenCellId();
    }

    bool Generator::checkIfRemoveFromBacktrackingHistory(const std::string& regionId) const {
        return !backtrackingHistory_.empty() && backtrackingHistory_.top().getRegionId() == regionId;
    }

    const std::vector<std::reference_wrapper<LatinSquare::Cell>> Generator::findRandomTransversal(
        LatinSquare::LatinSquare& latinSquare) {
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());
        std::vector<std::reference_wrapper<LatinSquare::Cell>> transversal;
        const unsigned size = latinSquare.getSize();
        int counter = 0;

        while (transversal.size() < size) {
            auto& region = latinSquare.getEnabledRegionWithMinimumEntropy();

            if (region.getEntropy() > 0) {
                counter = 0;

                const auto cells = region.getEnabledCells();
                auto iterator = cells.cbegin();
                std::advance(iterator, mersenneTwister() % cells.size());
                auto& cell = (*iterator).get();

                transversal.emplace_back(*iterator);
                const auto relatedCells = latinSquare.getCellsRelatedToChosenCell(cell);
                latinSquare.disableRelatedCells(relatedCells);
                latinSquare.decreaseRelatedRegionsEntropy(relatedCells);
                const auto disabledCellsIds = latinSquare.getDisabledCellsIds(relatedCells, cell);
                const auto relatedRegions = latinSquare.getRelatedRegions(cell);
                latinSquare.disableRelatedRegions(relatedRegions);

                updateHistory_.push(UpdateData(cell, disabledCellsIds));

                if (checkIfAddToBacktrackingHistory(cell)) {
                    backtrackingHistory_.push(BacktrackingData(region, cell));
                }
            } else {
                ++counter;

                if (counter > 1) {
                    auto backtrackingData = backtrackingHistory_.top();
                    backtrackingHistory_.pop();

                    std::set<std::string> disabledCellsIds = {backtrackingData.getChosenCellId()};
                    const auto& regionId = backtrackingData.getRegionId();

                    while (checkIfRemoveFromBacktrackingHistory(regionId)) {
                        backtrackingData = backtrackingHistory_.top();
                        backtrackingHistory_.pop();

                        disabledCellsIds.insert(backtrackingData.getChosenCellId());
                    }

                    const auto disabledCells = latinSquare.getCells(disabledCellsIds);
                    latinSquare.enableDisabledCells(disabledCells);
                    latinSquare.increaseRelatedRegionsEntropy(disabledCells);

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                transversal.pop_back();
                const auto updateData = updateHistory_.top();
                updateHistory_.pop();

                const auto disabledCells = latinSquare.getCells(updateData.getDisabledCellsIds());
                latinSquare.enableDisabledCells(disabledCells);
                latinSquare.increaseRelatedRegionsEntropy(disabledCells);
                const auto relatedRegions = latinSquare.getRelatedRegions(
                    latinSquare.getCell(updateData.getChosenCellId()));
                latinSquare.enableRelatedRegions(relatedRegions);
            }
        }

        return transversal;
    }
}
