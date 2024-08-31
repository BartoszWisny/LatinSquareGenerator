#include "LatinSquareGenerator.hpp"

#include <iterator>
#include <random>

namespace LatinSquareGenerator {
    const LatinSquare LatinSquareGenerator::generateRandomLatinSquare(const int size) {
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());
        auto latinSquare = LatinSquare(size, mersenneTwister);

        while (latinSquare.checkIfNotFilledCellExists()) {
            auto& cell = latinSquare.getNotFilledCellWithMinimumEntropy();

            if (cell.getEntropy() > 0) {
                const auto& remainingNumbers = cell.getRemainingNumbers();
                auto iterator = remainingNumbers.cbegin();
                std::advance(iterator, mersenneTwister() % remainingNumbers.size());
                const auto previousEntropyData = cell.getEntropyData();

                cell.fill(*iterator);
                updateHistory_.push(latinSquare.getUpdateData(cell, previousEntropyData));
            } else {
                if (updateHistory_.empty()) {
                    latinSquare.reset();
                    continue;
                }

                const auto updateData = updateHistory_.top();
                updateHistory_.pop();

                latinSquare.getCell(updateData.getFilledCellId()).clear(updateData.getPreviousEntropyData());
                const auto previousUpdatedCells = latinSquare.getPreviousUpdatedCells(updateData.getUpdatedCellsIds());
                const auto number = updateData.getFilledCellNumber();

                for (const auto& previousUpdatedCellRef : previousUpdatedCells) {
                    previousUpdatedCellRef.get().restoreRemainingNumber(number);
                }
            }
        }

        return latinSquare;
    }
}
