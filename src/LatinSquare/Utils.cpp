#include "Utils.hpp"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>

#include <cpp/string.hpp>

namespace LatinSquare {
    void printIds(LatinSquare& latinSquare) noexcept {
        const auto& grid = latinSquare.grid();

        for (const auto& cell : grid) {
            std::cout.write(cell->id().c_str(), cell->id().size());
            std::cout.put('\n');
        }
    }

    void printBoard(LatinSquare& latinSquare) noexcept {
        const auto& grid = latinSquare.grid();
        std::string leftBar = "+", repeatedLeftBar, spaces, numberString;
        leftBar.append(std::string(static_cast<uint_fast8_t>(std::log10(latinSquare.size()) + 1) + 2, '-'));
        repeatedLeftBar = cpp::repeat(leftBar, latinSquare.size());
        uint8_t number;

        for (const auto& cell : grid) {
            if (cell->rawColumn() == 0) {
                std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                std::cout.put('+');
                std::cout.put('\n');
            }

            number = cell->number();
            spaces = std::string(static_cast<uint_fast8_t>(std::log10(latinSquare.size()))
                                 - static_cast<uint_fast8_t>(std::log10(++number)) + 1, ' ');
            numberString = std::to_string(number);
            std::cout.put('|');
            std::cout.write(spaces.c_str(), spaces.size());
            std::cout.write(numberString.c_str(), numberString.size());
            std::cout.put(' ');

            if (cell->rawColumn() == latinSquare.size() - 1) {
                std::cout.put('|');
                std::cout.put('\n');
            }
        }

        std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
        std::cout.put('+');
        std::cout.put('\n');
    }
}
