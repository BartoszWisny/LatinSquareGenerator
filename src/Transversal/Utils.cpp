#include "Utils.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

#include <cpp/iostream.hpp>
#include <cpp/string.hpp>

namespace Transversal {
    void sortTransversal(std::vector<std::reference_wrapper<LatinSquare::Cell>>& transversal) {
        std::sort(transversal.begin(), transversal.end(),
                  [](const auto& firstCellRef, const auto& secondCellRef) {
                      return firstCellRef.get().getRow() < secondCellRef.get().getRow();
                  });
    }

    void printTransversalFullIds(std::vector<std::reference_wrapper<LatinSquare::Cell>>& transversal) {
        if (transversal.empty()) {
            const std::string info = "Transversal was not found";

            std::cout.write(info.c_str(), info.size());
            std::cout.put('\n');
            std::cout.put('\n');
        } else {
            sortTransversal(transversal);

            std::string fullId;

            std::cout.put('\n');

            for (const auto& cellRef : transversal) {
                fullId = cellRef.get().getFullId();

                std::cout.write(fullId.c_str(), fullId.size());
                std::cout.put('\n');
            }

            std::cout.put('\n');
        }
    }

    bool checkIfTransversalCell(const LatinSquare::Cell& cell, const std::vector<std::string>& ids) {
        return std::find(ids.cbegin(), ids.cend(), cell.getFullId()) != ids.cend();
    }

    void printTransversalBoard(
        LatinSquare::LatinSquare& latinSquare, std::vector<std::reference_wrapper<LatinSquare::Cell>>& transversal) {
        if (transversal.empty()) {
            const std::string info = "Transversal was not found";

            std::cout.write(info.c_str(), info.size());
            std::cout.put('\n');
            std::cout.put('\n');
        } else {
            latinSquare.sortGrid();
            sortTransversal(transversal);

            const auto& grid = latinSquare.getGrid();
            const auto size = latinSquare.getSize();
            std::vector<std::string> ids;
            ids.reserve(transversal.size());

            for (const auto& cellRef : transversal) {
                ids.emplace_back(cellRef.get().getFullId());
            }

            const auto barLength = static_cast<int>(std::log10(size) + 1) + 2;
            std::string shortBar = std::string(barLength, '-'), longBar = "+", leftBar = "+", rightBar = shortBar,
                repeatedLeftBar, spaces, numberString;
            auto isTransversalCell = false, isTransversalPreviousCell = false;
            auto index = 0, column = 0, previousColumn = 0, difference = 0, number = 0;
            longBar.append(shortBar);
            longBar.append("+");
            leftBar.append(shortBar);
            rightBar.append("+");

            std::cout.put('\n');

            for (const auto& cell : grid) {
                isTransversalCell = checkIfTransversalCell(cell, ids);

                if (cell.getColumn() == 1) {
                    column = transversal[index].get().getColumn();

                    if (cell.getRow() == 1) {
                        repeatedLeftBar = cpp::repeat(leftBar, column - 1);
                        std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                        std::cout.write(cpp::bold_on().c_str(), cpp::bold_on().size());
                        std::cout.write(cpp::green_on().c_str(), cpp::green_on().size());
                        std::cout.write(longBar.c_str(), longBar.size());
                        std::cout.write(cpp::reset().c_str(), cpp::reset().size());

                        if (column != size) {
                            repeatedLeftBar = cpp::repeat(leftBar, size - column - 1);

                            std::cout.write(shortBar.c_str(), shortBar.size());
                            std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                            std::cout.put('+');
                        }

                        std::cout.put('\n');
                    } else {
                        const auto [firstColumn, secondColumn] = std::minmax(previousColumn, column);
                        difference = secondColumn - firstColumn - 1;
                        repeatedLeftBar = cpp::repeat(leftBar, firstColumn - 1);

                        std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                        std::cout.write(cpp::bold_on().c_str(), cpp::bold_on().size());
                        std::cout.write(cpp::green_on().c_str(), cpp::green_on().size());
                        std::cout.write(longBar.c_str(), longBar.size());
                        std::cout.write(cpp::reset().c_str(), cpp::reset().size());

                        if (difference == 0) {
                            std::cout.write(cpp::bold_on().c_str(), cpp::bold_on().size());
                            std::cout.write(cpp::green_on().c_str(), cpp::green_on().size());
                            std::cout.write(rightBar.c_str(), rightBar.size());
                            std::cout.write(cpp::reset().c_str(), cpp::reset().size());
                        } else {
                            repeatedLeftBar = cpp::repeat(leftBar, difference - 1);

                            std::cout.write(shortBar.c_str(), shortBar.size());
                            std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                            std::cout.write(cpp::bold_on().c_str(), cpp::bold_on().size());
                            std::cout.write(cpp::green_on().c_str(), cpp::green_on().size());
                            std::cout.write(longBar.c_str(), longBar.size());
                            std::cout.write(cpp::reset().c_str(), cpp::reset().size());
                        }

                        if (secondColumn != size) {
                            repeatedLeftBar = cpp::repeat(leftBar, size - secondColumn - 1);

                            std::cout.write(shortBar.c_str(), shortBar.size());
                            std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                            std::cout.put('+');
                        }

                        std::cout.put('\n');
                    }
                }

                number = cell.getNumber();
                spaces =
                    std::string(static_cast<int>(std::log10(size)) - static_cast<int>(std::log10(number)) + 1, ' ');
                numberString = std::to_string(number);

                if (isTransversalCell) {
                    std::cout.write(cpp::bold_on().c_str(), cpp::bold_on().size());
                    std::cout.write(cpp::green_on().c_str(), cpp::green_on().size());
                    std::cout.put('|');
                    std::cout.write(spaces.c_str(), spaces.size());
                    std::cout.write(numberString.c_str(), numberString.size());
                    std::cout.put(' ');
                    std::cout.put('|');
                    std::cout.write(cpp::reset().c_str(), cpp::reset().size());
                } else {
                    if (!isTransversalPreviousCell) {
                        std::cout.put('|');
                    }

                    std::cout.write(spaces.c_str(), spaces.size());
                    std::cout.write(numberString.c_str(), numberString.size());
                    std::cout.put(' ');
                }

                if (cell.getColumn() == size) {
                    if (!isTransversalCell) {
                        std::cout.put('|');
                    }

                    std::cout.put('\n');

                    isTransversalPreviousCell = false;
                    previousColumn = column;
                    ++index;
                } else {
                    isTransversalPreviousCell = isTransversalCell;
                }
            }

            repeatedLeftBar = cpp::repeat(leftBar, column - 1);

            std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
            std::cout.write(cpp::bold_on().c_str(), cpp::bold_on().size());
            std::cout.write(cpp::green_on().c_str(), cpp::green_on().size());
            std::cout.write(longBar.c_str(), longBar.size());
            std::cout.write(cpp::reset().c_str(), cpp::reset().size());

            if (column != size) {
                repeatedLeftBar = cpp::repeat(leftBar, size - column - 1);

                std::cout.write(shortBar.c_str(), shortBar.size());
                std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                std::cout.put('+');
            }

            std::cout.put('\n');
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
