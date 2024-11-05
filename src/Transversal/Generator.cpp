#include "Generator.hpp"

namespace Transversal {
    bool Generator::checkIfAddToBacktrackingHistory(const LatinSquare::Cell& cell) const {
        return backtrackingHistory_.empty() || cell.getId() != backtrackingHistory_.top().getChosenCellId();
    }

    bool Generator::checkIfRemoveFromBacktrackingHistory(const std::string& regionId) const {
        return !backtrackingHistory_.empty() && backtrackingHistory_.top().getRegionId() == regionId;
    }

    const std::vector<std::reference_wrapper<LatinSquare::Cell>> Generator::findRandomTransversal(
        LatinSquare::LatinSquare& latinSquare, std::mt19937& mersenneTwister) {
        const unsigned size = latinSquare.getSize();
        std::vector<std::reference_wrapper<LatinSquare::Cell>> transversal;
        transversal.reserve(size);
        int counter = 0;

        while (transversal.size() < size) {
            auto& region = latinSquare.getRandomEnabledRegionWithMinimumEntropy();

            if (region.getEntropy() > 0) {
                counter = 0;

                const auto cells = region.getEnabledCells();
                auto& cell = cells[mersenneTwister() % cells.size()].get();

                transversal.emplace_back(cell);
                const auto relatedCells = latinSquare.getCellsRelatedToChosenCell(cell);
                latinSquare.disableRelatedCells(relatedCells);
                latinSquare.decreaseRelatedRegionsEntropy(relatedCells);
                const auto disabledCellsIds = latinSquare.getDisabledCellsIds(relatedCells, cell);
                const auto relatedRegions = latinSquare.getRelatedRegions(cell);
                latinSquare.disableRelatedRegions(relatedRegions);

                updateHistory_.emplace(cell, disabledCellsIds);

                if (checkIfAddToBacktrackingHistory(cell)) {
                    backtrackingHistory_.emplace(region, cell);
                }
            } else {
                ++counter;

                if (counter > 1) {
                    auto backtrackingData = backtrackingHistory_.top();
                    backtrackingHistory_.pop();

                    std::vector<std::string> disabledCellsIds;
                    disabledCellsIds.reserve(size);
                    disabledCellsIds.emplace_back(backtrackingData.getChosenCellId());

                    const auto& regionId = backtrackingData.getRegionId();

                    while (checkIfRemoveFromBacktrackingHistory(regionId)) {
                        backtrackingData = backtrackingHistory_.top();
                        backtrackingHistory_.pop();

                        disabledCellsIds.emplace_back(backtrackingData.getChosenCellId());
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

    const boost::multiprecision::uint512_t /* uint64_t */ Generator::countAllTransversals(
        LatinSquare::LatinSquare& latinSquare) {
        const unsigned size = latinSquare.getSize();
        std::vector<std::reference_wrapper<LatinSquare::Cell>> transversal;
        transversal.reserve(size);
        boost::multiprecision::uint512_t numberOfTransversals = 0;
        int counter = 0;

        while (true) {
            if (transversal.size() < size) {
                auto& region = latinSquare.getEnabledRegionWithMinimumEntropy();

                if (region.getEntropy() > 0) {
                    counter = 0;

                    auto& cell = region.getEnabledCells()[0].get();

                    transversal.emplace_back(cell);
                    const auto relatedCells = latinSquare.getCellsRelatedToChosenCell(cell);
                    latinSquare.disableRelatedCells(relatedCells);
                    latinSquare.decreaseRelatedRegionsEntropy(relatedCells);
                    const auto disabledCellsIds = latinSquare.getDisabledCellsIds(relatedCells, cell);
                    const auto relatedRegions = latinSquare.getRelatedRegions(cell);
                    latinSquare.disableRelatedRegions(relatedRegions);

                    updateHistory_.emplace(cell, disabledCellsIds);

                    if (checkIfAddToBacktrackingHistory(cell)) {
                        backtrackingHistory_.emplace(region, cell);
                    }
                } else {
                    ++counter;

                    if (counter > 1) {
                        auto backtrackingData = backtrackingHistory_.top();
                        backtrackingHistory_.pop();

                        std::vector<std::string> disabledCellsIds;
                        disabledCellsIds.reserve(size);
                        disabledCellsIds.emplace_back(backtrackingData.getChosenCellId());

                        const auto& regionId = backtrackingData.getRegionId();

                        while (checkIfRemoveFromBacktrackingHistory(regionId)) {
                            backtrackingData = backtrackingHistory_.top();
                            backtrackingHistory_.pop();

                            disabledCellsIds.emplace_back(backtrackingData.getChosenCellId());
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
            } else {
                ++numberOfTransversals;
                ++counter;

                // if (counter > 1) {
                //     auto backtrackingData = backtrackingHistory_.top();
                //     backtrackingHistory_.pop();

                //     std::vector<std::string> disabledCellsIds;
                //     disabledCellsIds.reserve(size);
                //     disabledCellsIds.emplace_back(backtrackingData.getChosenCellId());

                //     const auto& regionId = backtrackingData.getRegionId();

                //     while (checkIfRemoveFromBacktrackingHistory(regionId)) {
                //         backtrackingData = backtrackingHistory_.top();
                //         backtrackingHistory_.pop();

                //         disabledCellsIds.emplace_back(backtrackingData.getChosenCellId());
                //     }

                //     const auto disabledCells = latinSquare.getCells(disabledCellsIds);
                //     latinSquare.enableDisabledCells(disabledCells);
                //     latinSquare.increaseRelatedRegionsEntropy(disabledCells);

                //     if (updateHistory_.empty()) {
                //         break;
                //     }
                // }

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

        return numberOfTransversals;
    }
}
