#include "LatinSquareGenerator.hpp"

#include <algorithm>
#include <random>
#include <string>

#include <iostream>

namespace LatinSquareGenerator {
    LatinSquareGenerator::LatinSquareGenerator() {}

    LatinSquare LatinSquareGenerator::generateRandomLatinSquare(const int size) {
        auto latinSquare = LatinSquare(size);
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());

        while (latinSquare.checkIfNotFilledCellExists()) {
            auto& cell = latinSquare.getNotFilledCellWithMinimumEntropy();

            if (cell.getEntropy() > 0) {
                auto remainingNumbers = cell.getRemainingNumbers();
                std::vector<int> numbers;
                std::sample(
                    remainingNumbers.begin(), remainingNumbers.end(), std::back_inserter(numbers), 1, mersenneTwister);
                auto number = numbers.front();

                auto updateData = latinSquare.getUpdateData(
                    cell.getId(), cell.getRow(), cell.getColumn(), number, cell.getEntropyData());
                updateHistory_.push(updateData);
                cell.fill(number);
            } else {
                if (updateHistory_.empty()) {
                    latinSquare.reset();
                }

                auto updateData = updateHistory_.top();
                auto filledCellData = updateData.getFilledCellData();
                auto updatedCellsIds = updateData.getUpdatedCellsIds();
                updateHistory_.pop();

                auto& previousCell = latinSquare.getCell(filledCellData.getId());
                previousCell.clear(filledCellData.getPreviousEntropyData());
                auto previousUpdatedCells = latinSquare.getPreviousUpdatedCells(updatedCellsIds);
                auto number = filledCellData.getNumber();

                for (auto previousUpdatedCell : previousUpdatedCells) {
                    previousUpdatedCell.restoreRemainingNumber(number);
                }
            }
        }

        return latinSquare;
    }
}
