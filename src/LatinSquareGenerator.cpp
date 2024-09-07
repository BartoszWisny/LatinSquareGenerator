#include "LatinSquareGenerator.hpp"

#include <iterator>
#include <random>

namespace LatinSquareGenerator {
    bool LatinSquareGenerator::checkIfAddToBacktrackingHistory(const Cell& cell) const {
        return backtrackingHistory_.empty() || cell.getId() != backtrackingHistory_.top().getFilledCellId();
    }

    const LatinSquare LatinSquareGenerator::generateRandomLatinSquare(const int size) {
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());
        auto latinSquare = LatinSquare(size, mersenneTwister);
        int counter = 0;

        while (latinSquare.checkIfNotFilledCellExists()) {
            auto& cell = latinSquare.getNotFilledCellWithMinimumEntropy();

            if (cell.getEntropy() > 0) {
                counter = 0;

                const auto& remainingNumbers = cell.getRemainingNumbers();
                auto iterator = remainingNumbers.cbegin();
                std::advance(iterator, mersenneTwister() % remainingNumbers.size());
                const auto previousEntropyData = cell.getEntropyData();

                cell.fill(*iterator);
                updateHistory_.push(latinSquare.getUpdateData(cell, previousEntropyData));

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
                const auto cells = latinSquare.getCells(updateData.getUpdatedCellsIds());
                const auto number = updateData.getFilledCellNumber();

                for (const auto& cellRef : cells) {
                    cellRef.get().restoreRemainingNumber(number);
                }
            }
        }

        return latinSquare;
    }
}
