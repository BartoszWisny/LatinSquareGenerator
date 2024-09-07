#include "LatinSquareGenerator.hpp"

#include <iterator>
#include <random>



#include <iostream>



namespace LatinSquareGenerator {
    bool LatinSquareGenerator::checkIfAddToBacktrackingHistory(const Cell& cell) const {
        return backtrackingHistory_.empty() || cell.getId() != backtrackingHistory_.top().getFilledCellId();
    }

    const LatinSquare LatinSquareGenerator::generateRandomLatinSquare(const int size) {
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());
        auto latinSquare = LatinSquare(size, mersenneTwister);
        int counter = 0;

        int it = 0;

        while (latinSquare.checkIfNotFilledCellExists()) {
            auto& cell = latinSquare.getNotFilledCellWithMinimumEntropy();

            if (cell.getEntropy() > 0 && (it % 5 != 2 && it % 5 != 3)) {
                ++it;

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

                // ---

                std::cout << it << ". iteration" << std::endl;
                std::cout << "Filled cell: " << cell.getId() << " with number: " << cell.getNumber() << std::endl;
                std::cout << "Updated cells: ";

                for (const std::string& updatedCellId : updateHistory_.top().getUpdatedCellsIds()) {
                    std::cout << updatedCellId << " ";
                }

                std::cout << std::endl;

                for (const auto& cell : latinSquare.getGrid()) {
                    std::cout << cell.getId() << ": number: " << cell.getNumber() << ", entropy: " << cell.getEntropy()
                              << ", remaining numbers: ";

                    for (const int number : cell.getRemainingNumbers()) {
                        std::cout << number << " ";
                    }

                    std::cout << std::endl;
                }

                std::cout << std::endl;

                // ---
            } else { // TODO: check if works correctly (in case of multiple time backtracking)
                ++it;

                ++counter;

                if (counter > 1) {
                    std::cout << "Backtracking history size: " << backtrackingHistory_.size() << ", counter: " << counter << std::endl;

                    const auto backtrackingData = backtrackingHistory_.top();
                    backtrackingHistory_.pop();

                    latinSquare.getCell(backtrackingData.getFilledCellId()).setEntropyData(
                        backtrackingData.getPreviousEntropyData());

                    std::cout << it << ". iteration (BACKTRACKING)" << std::endl;

                    for (const auto& cell : latinSquare.getGrid()) {
                        std::cout << cell.getId() << ": number: " << cell.getNumber() << ", entropy: " << cell.getEntropy()
                                << ", remaining numbers: ";

                        for (const int number : cell.getRemainingNumbers()) {
                            std::cout << number << " ";
                        }

                        std::cout << std::endl;
                    }

                    std::cout << std::endl;

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

                // ---

                std::cout << it << ". iteration (BACKTRACKING)" << std::endl;
                std::cout << "Previous cell: " << updateData.getFilledCellId() << " with number: " << number
                          << std::endl;

                for (const auto& cell : latinSquare.getGrid()) {
                    std::cout << cell.getId() << ": number: " << cell.getNumber() << ", entropy: " << cell.getEntropy()
                              << ", remaining numbers: ";

                    for (const int number : cell.getRemainingNumbers()) {
                        std::cout << number << " ";
                    }

                    std::cout << std::endl;
                }

                std::cout << std::endl;

                // ---
            }
        }

        return latinSquare;
    }
}
