#include "Utils.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

#include <cpp/iostream.hpp>
#include <cpp/string.hpp>

namespace Transversal {
    void printIds(LatinSquare::LatinSquare& latinSquare, std::vector<uint_fast16_t>& transversal) noexcept {
        if (transversal.empty()) {
            const std::string info = "Transversal was not found";
            std::cout.write(info.c_str(), info.size());
            std::cout.put('\n');
        } else {
            const auto& grid = latinSquare.grid();
            std::sort(transversal.begin(), transversal.end());
            std::string id;

            for (const auto& index : transversal) {
                id = grid[index]->id();
                std::cout.write(id.c_str(), id.size());
                std::cout.put('\n');
            }
        }
    }

    void printBoard(LatinSquare::LatinSquare& latinSquare, std::vector<uint_fast16_t>& transversal) noexcept {
        if (transversal.empty()) {
            const std::string info = "Transversal was not found";
            std::cout.write(info.c_str(), info.size());
            std::cout.put('\n');
        } else {
            const auto& grid = latinSquare.grid();
            std::sort(transversal.begin(), transversal.end());
            const auto barLength = static_cast<uint_fast8_t>(std::log10(latinSquare.size()) + 1) + 2;
            std::string shortBar = std::string(barLength, '-'), longBar = "+", leftBar = "+", rightBar = shortBar,
                repeatedLeftBar, spaces, numberString;
            longBar.append(shortBar);
            longBar.append("+");
            leftBar.append(shortBar);
            rightBar.append("+");
            auto isTransversalCell = false, isTransversalPreviousCell = false;
            uint_fast16_t index = 0;
            uint_fast8_t column = 0, previousColumn = 0, difference = 0, number = 0;

            for (const auto& cell : grid) {
                isTransversalCell =
                    (std::find(transversal.cbegin(), transversal.cend(), cell->index()) != transversal.cend());

                if (cell->rawColumn() == 0) {
                    column = grid[transversal[index]]->rawColumn();

                    if (cell->rawRow() == 0) {
                        repeatedLeftBar = cpp::repeat(leftBar, column);
                        std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                        std::cout.write(cpp::bold_on().c_str(), cpp::bold_on().size());
                        std::cout.write(cpp::green_on().c_str(), cpp::green_on().size());
                        std::cout.write(longBar.c_str(), longBar.size());
                        std::cout.write(cpp::reset().c_str(), cpp::reset().size());

                        if (column != latinSquare.size() - 1) {
                            repeatedLeftBar = cpp::repeat(leftBar, latinSquare.size() - column - 2);
                            std::cout.write(shortBar.c_str(), shortBar.size());
                            std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                            std::cout.put('+');
                        }

                        std::cout.put('\n');
                    } else {
                        const auto [firstColumn, secondColumn] = std::minmax(previousColumn, column);
                        difference = secondColumn - firstColumn - 1;
                        repeatedLeftBar = cpp::repeat(leftBar, firstColumn);
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

                        if (secondColumn != latinSquare.size() - 1) {
                            repeatedLeftBar = cpp::repeat(leftBar, latinSquare.size() - secondColumn - 2);
                            std::cout.write(shortBar.c_str(), shortBar.size());
                            std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                            std::cout.put('+');
                        }

                        std::cout.put('\n');
                    }
                }

                number = cell->number();
                spaces = std::string(static_cast<uint_fast8_t>(std::log10(latinSquare.size()))
                                     - static_cast<uint_fast8_t>(std::log10(++number)) + 1, ' ');
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

                if (cell->rawColumn() == latinSquare.size() - 1) {
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

            repeatedLeftBar = cpp::repeat(leftBar, column);
            std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
            std::cout.write(cpp::bold_on().c_str(), cpp::bold_on().size());
            std::cout.write(cpp::green_on().c_str(), cpp::green_on().size());
            std::cout.write(longBar.c_str(), longBar.size());
            std::cout.write(cpp::reset().c_str(), cpp::reset().size());

            if (column != latinSquare.size() - 1) {
                repeatedLeftBar = cpp::repeat(leftBar, latinSquare.size() - column - 2);
                std::cout.write(shortBar.c_str(), shortBar.size());
                std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                std::cout.put('+');
            }

            std::cout.put('\n');
        }
    }
}
