#include "Utils.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <iterator>

#include <cpp/iostream.hpp>

namespace Transversal {
    void sortTransversal(std::vector<std::reference_wrapper<LatinSquare::Cell>>& transversal) {
        std::sort(transversal.begin(), transversal.end(),
                  [](const auto& firstCellRef, const auto& secondCellRef) {
                      return firstCellRef.get().getRow() < secondCellRef.get().getRow();
                  });
    }

    void printTransversalFullIds(std::vector<std::reference_wrapper<LatinSquare::Cell>>& transversal) {
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

    bool checkIfTransversalCell(const LatinSquare::Cell& cell, const std::set<std::string>& ids) {
        return ids.contains(cell.getFullId());
    }

    void printTransversalBoard(
        LatinSquare::LatinSquare& latinSquare, std::vector<std::reference_wrapper<LatinSquare::Cell>>& transversal) {
        if (transversal.empty()) {
            std::cout << "Transversal was not found" << std::endl << std::endl;
        } else {
            latinSquare.sortGrid();
            sortTransversal(transversal);

            const auto& grid = latinSquare.getGrid();
            const auto size = latinSquare.getSize();
            std::set<std::string> ids;
            std::transform(transversal.cbegin(), transversal.cend(), std::inserter(ids, ids.cend()),
                           [](const auto& cellRef) { return cellRef.get().getFullId(); });

            const auto barLength = int(log10(size) + 1) + 2;
            const auto longBar = std::format("+{}+", std::string(barLength, '-'));
            const auto leftBar = std::format("+{}", std::string(barLength, '-'));
            const auto rightBar = std::format("{}+", std::string(barLength, '-'));
            const auto shortBar = std::format("{}", std::string(barLength, '-'));
            auto isTransversalCell = false;
            auto isTransversalPreviousCell = false;
            auto index = 0;
            auto column = 0;
            auto previousColumn = 0;
            auto difference = 0;
            auto number = 0;
            std::string spaces = "";

            std::cout << std::endl;

            for (const auto& cell : grid) {
                isTransversalCell = checkIfTransversalCell(cell, ids);

                if (cell.getColumn() == 1) {
                    column = transversal.at(index).get().getColumn();

                    if (cell.getRow() == 1) {
                        std::fill_n(std::ostream_iterator<std::string>(std::cout), column - 1, leftBar);
                        std::cout << cpp::bold_on << cpp::green_on << longBar << cpp::reset;

                        if (column != size) {
                            std::cout << shortBar;
                            std::fill_n(std::ostream_iterator<std::string>(std::cout), size - column - 1, leftBar);
                            std::cout << "+";
                        }

                        std::cout << std::endl;
                    } else {
                        const auto [firstColumn, secondColumn] = std::minmax(previousColumn, column);
                        difference = secondColumn - firstColumn - 1;

                        std::fill_n(std::ostream_iterator<std::string>(std::cout), firstColumn - 1, leftBar);
                        std::cout << cpp::bold_on << cpp::green_on << longBar << cpp::reset;

                        if (difference == 0) {
                            std::cout << cpp::bold_on << cpp::green_on << rightBar << cpp::reset;
                        } else {
                            std::cout << shortBar;
                            std::fill_n(std::ostream_iterator<std::string>(std::cout), difference - 1, leftBar);
                            std::cout << cpp::bold_on << cpp::green_on << longBar << cpp::reset;
                        }

                        if (secondColumn != size) {
                            std::cout << shortBar;
                            std::fill_n(std::ostream_iterator<std::string>(std::cout), size - secondColumn - 1,
                                        leftBar);
                            std::cout << "+";
                        }

                        std::cout << std::endl;
                    }
                }

                number = cell.getNumber();
                spaces = std::string(int(log10(size)) - int(log10(number)) + 1, ' ');

                if (isTransversalCell) {
                    std::cout << cpp::bold_on << cpp::green_on << "|" << spaces << number << " |" << cpp::reset;
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
                    previousColumn = column;
                    ++index;
                } else {
                    isTransversalPreviousCell = isTransversalCell;
                }
            }

            std::fill_n(std::ostream_iterator<std::string>(std::cout), column - 1, leftBar);
            std::cout << cpp::bold_on << cpp::green_on << longBar << cpp::reset;

            if (column != size) {
                std::cout << shortBar;
                std::fill_n(std::ostream_iterator<std::string>(std::cout), size - column - 1,
                            leftBar);
                std::cout << "+";
            }

            std::cout << std::endl;
        }
    }
}

// Might be helpful:

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
