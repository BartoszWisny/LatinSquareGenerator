#include "LatinSquarePrintingUtils.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <iterator>

#include <cpp_utils.hpp>

#include "LatinSquare.hpp"

namespace LatinSquareGenerator {
    void printLatinSquareFullIds(LatinSquare& latinSquare) {
        latinSquare.sortGridByRows();
        const auto& grid = latinSquare.getGrid();

        std::cout << std::endl;

        for (const auto& cell : grid) {
            std::cout << cell.getFullId() << std::endl;
        }

        std::cout << std::endl;
    }

    void printLatinSquareBoard(LatinSquare& latinSquare) {
        latinSquare.sortGridByRows();

        const auto& grid = latinSquare.getGrid();
        const auto size = latinSquare.getSize();

        const auto leftHorizontalBar = std::format("+{}", std::string(int(log10(size) + 1) + 2, '-'));
        auto number = 0;
        std::string spaces = "";

        std::cout << std::endl;

        for (const auto& cell : grid) {
            if (cell.getColumn() == 1) {
                std::fill_n(std::ostream_iterator<std::string>(std::cout), size, leftHorizontalBar);
                std::cout << "+" << std::endl;
            }

            number = cell.getNumber();
            spaces = std::string(int(log10(size)) - int(log10(number)) + 1, ' ');

            std::cout << "|" << spaces << number << " ";

            if (cell.getColumn() == size) {
                std::cout << "|" << std::endl;
            }
        }

        std::fill_n(std::ostream_iterator<std::string>(std::cout), size, leftHorizontalBar);
        std::cout << "+" << std::endl;
        std::cout << std::endl;
    }

    void sortTransversal(std::vector<std::reference_wrapper<Cell>>& transversal) {
        std::sort(transversal.begin(), transversal.end(),
                  [](const auto& firstCellRef, const auto& secondCellRef) {
                      return firstCellRef.get().getRow() < secondCellRef.get().getRow();
                  });
    }

    void printTransversalFullIds(std::vector<std::reference_wrapper<Cell>>& transversal) {
        if (transversal.empty()) {
            std::cout << "Transversal was not found" << std::endl << std::endl;
        } else {
            sortTransversal(transversal);

            std::cout << std::endl;

            for (const auto& cellRef : transversal) {
                std::cout << cellRef.get().getFullId() << std::endl;
            }

            std::cout << std::endl;
        }
    }

    bool checkIfTransversalCell(const Cell& cell, const std::set<std::string>& transversalFullIds) {
        return transversalFullIds.contains(cell.getFullId());
    }

    void printTransversalBoard(LatinSquare& latinSquare, std::vector<std::reference_wrapper<Cell>>& transversal) {
        if (transversal.empty()) {
            std::cout << "Transversal was not found" << std::endl << std::endl;
        } else {
            latinSquare.sortGridByRows();
            sortTransversal(transversal);

            const auto& grid = latinSquare.getGrid();
            const auto size = latinSquare.getSize();
            std::set<std::string> transversalFullIds;
            std::transform(transversal.cbegin(), transversal.cend(),
                           std::inserter(transversalFullIds, transversalFullIds.cend()),
                           [](const auto& cellRef) { return cellRef.get().getFullId(); });

            const auto barLength = int(log10(size) + 1) + 2;
            const auto longHorizontalBar = std::format("+{}+", std::string(barLength, '-'));
            const auto leftHorizontalBar = std::format("+{}", std::string(barLength, '-'));
            const auto rightHorizontalBar = std::format("{}+", std::string(barLength, '-'));
            const auto shortHorizontalBar = std::format("{}", std::string(barLength, '-'));
            auto isTransversalCell = false;
            auto isTransversalPreviousCell = false;
            auto transversalIndex = 0;
            auto transversalCellColumn = 0;
            auto transversalPreviousCellColumn = 0;
            auto columnsDifference = 0;
            auto number = 0;
            std::string spaces = "";

            std::cout << std::endl;

            for (const auto& cell : grid) {
                isTransversalCell = checkIfTransversalCell(cell, transversalFullIds);

                if (cell.getColumn() == 1) {
                    transversalCellColumn = transversal.at(transversalIndex).get().getColumn();

                    if (cell.getRow() == 1) {
                        std::fill_n(std::ostream_iterator<std::string>(std::cout), transversalCellColumn - 1,
                                    leftHorizontalBar);
                        std::cout << cpp_utils::bold_on << cpp_utils::green_on << longHorizontalBar << cpp_utils::reset;

                        if (transversalCellColumn != size) {
                            std::cout << shortHorizontalBar;
                            std::fill_n(std::ostream_iterator<std::string>(std::cout), size - transversalCellColumn - 1,
                                        leftHorizontalBar);
                            std::cout << "+";
                        }

                        std::cout << std::endl;
                    } else {
                        const auto [firstColumn, secondColumn] = std::minmax(
                            transversalPreviousCellColumn, transversalCellColumn);
                        columnsDifference = secondColumn - firstColumn - 1;

                        std::fill_n(std::ostream_iterator<std::string>(std::cout), firstColumn - 1, leftHorizontalBar);
                        std::cout << cpp_utils::bold_on << cpp_utils::green_on << longHorizontalBar << cpp_utils::reset;

                        if (columnsDifference == 0) {
                            std::cout << cpp_utils::bold_on << cpp_utils::green_on << rightHorizontalBar
                                      << cpp_utils::reset;
                        } else {
                            std::cout << shortHorizontalBar;
                            std::fill_n(std::ostream_iterator<std::string>(std::cout), columnsDifference - 1,
                                        leftHorizontalBar);
                            std::cout << cpp_utils::bold_on << cpp_utils::green_on << longHorizontalBar
                                      << cpp_utils::reset;
                        }

                        if (secondColumn != size) {
                            std::cout << shortHorizontalBar;
                            std::fill_n(std::ostream_iterator<std::string>(std::cout), size - secondColumn - 1,
                                        leftHorizontalBar);
                            std::cout << "+";
                        }

                        std::cout << std::endl;
                    }
                }

                number = cell.getNumber();
                spaces = std::string(int(log10(size)) - int(log10(number)) + 1, ' ');

                if (isTransversalCell) {
                    std::cout << cpp_utils::bold_on << cpp_utils::green_on << "|" << spaces << number << " |"
                              << cpp_utils::reset;
                } else {
                    if (!isTransversalPreviousCell) {
                        std::cout << "|";
                    }

                    std::cout << spaces << number << " ";
                }

                if (cell.getColumn() == size) {
                    if (!isTransversalCell) {
                        std::cout << "|";
                    }

                    std::cout << std::endl;

                    isTransversalPreviousCell = false;
                    transversalPreviousCellColumn = transversalCellColumn;
                    ++transversalIndex;
                } else {
                    isTransversalPreviousCell = isTransversalCell;
                }
            }

            std::fill_n(std::ostream_iterator<std::string>(std::cout), transversalCellColumn - 1, leftHorizontalBar);
            std::cout << cpp_utils::bold_on << cpp_utils::green_on << longHorizontalBar << cpp_utils::reset;

            if (transversalCellColumn != size) {
                std::cout << shortHorizontalBar;
                std::fill_n(std::ostream_iterator<std::string>(std::cout), size - transversalCellColumn - 1,
                            leftHorizontalBar);
                std::cout << "+";
            }

            std::cout << std::endl;
        }
    }
}

// Might be helpful:

// 1)
// std::cout << it << ". iteration" << std::endl;
// std::cout << "Filled cell: " << cell.getId() << " with number: " << *iterator << std::endl;
// std::cout << "Updated cells: ";

// for (const std::string& updatedCellId : updateHistory_.top().getUpdatedCellsIds()) {
//     std::cout << updatedCellId << " ";
// }

// std::cout << std::endl;

// for (const auto& cell : latinSquare.getGrid()) {
//     std::cout << cell.getId() << ": number: " << cell.getNumber() << ", entropy: " << cell.getEntropy()
//               << ", remaining numbers: ";

//     for (const int number : cell.getRemainingNumbers()) {
//         std::cout << number << " ";
//     }

//     std::cout << std::endl;
// }

// std::cout << std::endl;

// 2)
// std::cout << it << ". iteration (BACKTRACKING)" << std::endl;
// std::cout << "Previous cell: " << updateData.getFilledCellId() << " with number: " << number
//           << std::endl;

// for (const auto& cell : latinSquare.getGrid()) {
//     std::cout << cell.getId() << ": number: " << cell.getNumber() << ", entropy: " << cell.getEntropy()
//               << ", remaining numbers: ";

//     for (const int number : cell.getRemainingNumbers()) {
//         std::cout << number << " ";
//     }

//     std::cout << std::endl;
// }

// std::cout << std::endl;


// And for transversals:

// 1)
// for (auto region : latinSquare.getRegions()) {
//     std::cout << region.getId() << ": entropy: " << region.getEntropy() << ", cells: ";
//     for (auto cell : region.getCells()) {
//         std::cout << cell.get().getFullId() << " ";
//     }
//     std::cout << ", enabled: " << region.isEnabled() << std::endl;
// }

// std::cout << std::endl;

// 2)
// std::cout << it << ". iteration" << std::endl;
// std::cout << "Chosen cell: " << cell.getFullId() << std::endl;
// std::cout << "Disabled cells: ";

// for (const auto& disabledCellId : updateHistory_.top().getDisabledCellsIds()) {
//     std::cout << disabledCellId << " ";
// }

// std::cout << std::endl;

// for (auto region : latinSquare.getRegions()) {
//     std::cout << region.getId() << ": entropy: " << region.getEntropy() << ", enabled cells: ";
//     const auto enabledCells = region.getEnabledCells();
//     std::set<std::string> enabledCellsIds;
//     std::transform(enabledCells.cbegin(), enabledCells.cend(),
//                     std::inserter(enabledCellsIds, enabledCellsIds.cend()),
//                     [](const auto& cellRef) { return cellRef.get().getFullId(); });

//     for (auto cellId : enabledCellsIds) {
//         std::cout << cellId << " ";
//     }

//     std::cout << ", enabled: " << region.isEnabled() << std::endl;
// }

// std::cout << std::endl;

// 3)
// std::cout << it << ". iteration (BACKTRACKING)" << std::endl;
// std::cout << "Previous cell: " << updateData.getChosenCellId() << std::endl;

// for (auto region : latinSquare.getRegions()) {
//     std::cout << region.getId() << ": entropy: " << region.getEntropy() << ", enabled cells: ";
//     const auto enabledCells = region.getEnabledCells();
//     std::set<std::string> enabledCellsIds;
//     std::transform(enabledCells.cbegin(), enabledCells.cend(),
//                     std::inserter(enabledCellsIds, enabledCellsIds.cend()),
//                     [](const auto& cellRef) { return cellRef.get().getFullId(); });

//     for (auto cellId : enabledCellsIds) {
//         std::cout << cellId << " ";
//     }

//     std::cout << ", enabled: " << region.isEnabled() << std::endl;
// }

// std::cout << std::endl;
