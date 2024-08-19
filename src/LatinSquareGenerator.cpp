#include "LatinSquareGenerator.hpp"

#include <algorithm>
#include <random>
#include <string>

#include <iostream>

namespace LatinSquareGenerator {
    LatinSquareGenerator::LatinSquareGenerator() {}

    const LatinSquare LatinSquareGenerator::generateRandomLatinSquare(const int size) {
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());
        auto latinSquare = LatinSquare(size, mersenneTwister);
        // int it = 0;

        while (latinSquare.checkIfNotFilledCellExists()) {
            // it++;
            auto& cell = latinSquare.getNotFilledCellWithMinimumEntropy();

            if (cell.getEntropy() > 0) {
                const auto& remainingNumbers = cell.getRemainingNumbers();
                std::vector<int> numbers;
                std::sample(
                    remainingNumbers.begin(), remainingNumbers.end(), std::back_inserter(numbers), 1, mersenneTwister);
                const auto& number = numbers.front();

                const auto updateData = latinSquare.getUpdateData(
                    cell.getId(), cell.getRow(), cell.getColumn(), number, cell.getEntropyData());
                updateHistory_.push(updateData);
                cell.fill(number);

                // std::cout << it << ". iteration" << std::endl;
                // std::cout << "Filled cell: " << cell.getId() << " with number: " << number << std::endl;

                // for (const auto& cell : latinSquare.getGrid()) {
                //     std::cout << cell.getId() << ": number: " << cell.getNumber() << ", entropy: " << cell.getEntropy()
                //               << ", remaining numbers: ";

                //     for (const int number : cell.getRemainingNumbers()) {
                //         std::cout << number << " ";
                //     }

                //     std::cout << std::endl;
                // }

                // std::cout << std::endl;
            } else { // TODO: check if this case works correctly
                if (updateHistory_.empty()) {
                    latinSquare.reset();
                }

                const auto updateData = updateHistory_.top();
                const auto& filledCellData = updateData.getFilledCellData();
                const auto& updatedCellsIds = updateData.getUpdatedCellsIds();
                updateHistory_.pop();

                auto& previousCell = latinSquare.getCell(filledCellData.getId());
                previousCell.clear(filledCellData.getPreviousEntropyData());
                const auto previousUpdatedCells = latinSquare.getPreviousUpdatedCells(updatedCellsIds);
                const auto number = filledCellData.getNumber();

                for (const auto& previousUpdatedCellRef : previousUpdatedCells) {
                    previousUpdatedCellRef.get().restoreRemainingNumber(number);
                }
            }
        }

        return latinSquare;
    }
}
