#include "LatinSquareGenerator.hpp"

#include <algorithm>
#include <random>

namespace LatinSquareGenerator {
    LatinSquare LatinSquareGenerator::generateRandomLatinSquare(int size) {
        LatinSquare latinSquare = LatinSquare(size);
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());

        while (latinSquare.checkIfNotFilledCellExists()) {
            Cell cell = latinSquare.getNotFilledCellWithMinimumEntropy();

            if (cell.getEntropy() > 0) {
                std::set<int> remainingNumbers = cell.getRemainingNumbers();
                std::set<int> numbers;
                std::sample(remainingNumbers.begin(), remainingNumbers.end(), std::back_inserter(numbers), 1,
                            mersenneTwister);
                int number = *numbers.begin();

                UpdateData updateData = latinSquare.getUpdateData(cell.getPosition(), number, cell.getEntropyData());
                updateHistory.push(updateData);
                cell.fill(number);
            } else {
                if (updateHistory.empty()) {
                    break;
                }

                UpdateData updateData = updateHistory.top();
                FilledCellData filledCellData = updateData.getFilledCellData();
                std::vector<Position> updatedCells = updateData.getUpdatedCells();
                updateHistory.pop();

                Cell previousCell = latinSquare.getCell(filledCellData.getPosition());
                previousCell.clear(filledCellData.getPreviousEntropyData());

                // TODO: get cells with positions from update data
                // Then: for (Cell cell : cells) { cell.restoreRemainingNumber(filledCellData.getNumber()); }
            }
        }

        return latinSquare;
    }
}
