#include "Generator.hpp"

namespace LatinSquare {
    bool Generator::checkIfAddToBacktrackingHistory(const Cell& cell) const {
        return backtrackingHistory_.empty() || cell.getId() != backtrackingHistory_.top().getFilledCellId();
    }

    const LatinSquare Generator::generateRandomLatinSquare(
        const int size, const bool reduced, std::mt19937& mersenneTwister) {
        LatinSquare latinSquare(size, reduced, mersenneTwister);
        int counter = 0;

        while (latinSquare.checkIfNotFilledCellExists()) {
            auto& cell = latinSquare.getNotFilledCellWithMinimumEntropy();

            if (cell.getEntropy() > 0) {
                counter = 0;

                const auto& remainingNumbers = cell.getRemainingNumbers();
                const auto number = remainingNumbers[mersenneTwister() % remainingNumbers.size()];
                const auto previousEntropyData = cell.getEntropyData();

                cell.fill(number);
                auto relatedCells = latinSquare.getCellsRelatedToFilledCell(cell);
                latinSquare.updateRelatedCells(relatedCells, number);
                const auto updatedCellsIds = latinSquare.getUpdatedCellsIds(relatedCells);

                updateHistory_.emplace(cell, previousEntropyData, updatedCellsIds);

                if (checkIfAddToBacktrackingHistory(cell)) {
                    backtrackingHistory_.emplace(cell, previousEntropyData);
                }
            } else {
                ++counter;

                if (counter > 1) {
                    const auto backtrackingData = backtrackingHistory_.top();
                    backtrackingHistory_.pop();

                    latinSquare.getCell(backtrackingData.getFilledCellId()).setEntropyData(
                        backtrackingData.getPreviousEntropyData());

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                const auto updateData = updateHistory_.top();
                updateHistory_.pop();

                latinSquare.getCell(updateData.getFilledCellId()).clear(updateData.getPreviousEntropyData());
                const auto updatedCells = latinSquare.getCells(updateData.getUpdatedCellsIds());
                const auto number = updateData.getFilledCellNumber();
                latinSquare.restoreUpdatedCells(updatedCells, number);
            }
        }

        return latinSquare;
    }
}
