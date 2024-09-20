#include "Generator.hpp"

#include <iterator>
#include <random>

namespace LatinSquare {
    bool Generator::checkIfAddToBacktrackingHistory(const Cell& cell) const {
        return backtrackingHistory_.empty() || cell.getId() != backtrackingHistory_.top().getFilledCellId();
    }

    const LatinSquare Generator::generateRandomLatinSquare(const int size, const bool reduced) {
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());
        auto latinSquare = LatinSquare(size, reduced, mersenneTwister);
        latinSquare.shuffleGrid();
        int counter = 0;

        while (latinSquare.checkIfNotFilledCellExists()) {
            auto& cell = latinSquare.getNotFilledCellWithMinimumEntropy();

            if (cell.getEntropy() > 0) {
                counter = 0;

                const auto& remainingNumbers = cell.getRemainingNumbers();
                auto iterator = remainingNumbers.cbegin();
                std::advance(iterator, mersenneTwister() % remainingNumbers.size());
                const auto number = *iterator;
                const auto previousEntropyData = cell.getEntropyData();

                cell.fill(number);
                auto relatedCells = latinSquare.getCellsRelatedToFilledCell(cell);
                latinSquare.updateRelatedCells(relatedCells, number);
                const auto updatedCellsIds = latinSquare.getUpdatedCellsIds(relatedCells);

                updateHistory_.push(UpdateData(cell, previousEntropyData, updatedCellsIds));

                if (checkIfAddToBacktrackingHistory(cell)) {
                    backtrackingHistory_.push(BacktrackingData(cell, previousEntropyData));
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
