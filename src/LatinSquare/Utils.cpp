#include "Utils.hpp"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>

#include <cpp/string.hpp>

namespace LatinSquare {
    void printIds(LatinSquare& latinSquare) {
        const auto& grid = latinSquare.grid();

        std::string fullId;

        std::cout.put('\n');

        for (const auto& cell : grid) {
            fullId = cell->id();

            std::cout.write(fullId.c_str(), fullId.size());
            std::cout.put('\n');
        }

        std::cout.put('\n');
    }

    void printBoard(LatinSquare& latinSquare) {
        const auto& grid = latinSquare.grid();
        const auto size = latinSquare.size();

        std::string leftBar = "+", repeatedLeftBar, spaces, numberString;
        auto number = 0;
        leftBar.append(std::string(static_cast<int>(std::log10(size) + 1) + 2, '-'));
        repeatedLeftBar = cpp::repeat(leftBar, size);

        std::cout.put('\n');

        for (const auto& cell : grid) {
            if (cell->rawColumn() == 0) {
                std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                std::cout.put('+');
                std::cout.put('\n');
            }

            number = cell->number();
            spaces = std::string(static_cast<int>(std::log10(size)) - static_cast<int>(std::log10(++number)) + 1, ' ');
            numberString = std::to_string(number);

            std::cout.put('|');
            std::cout.write(spaces.c_str(), spaces.size());
            std::cout.write(numberString.c_str(), numberString.size());
            std::cout.put(' ');

            if (cell->rawColumn() == size - 1) {
                std::cout.put('|');
                std::cout.put('\n');
            }
        }

        std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
        std::cout.put('+');
        std::cout.put('\n');
        std::cout.put('\n');
    }
}

// Might be helpful:

// 1)
// std::cout << it << ". iteration" << std::endl;
// std::cout << "Filled cell: " << cell.id() << " with number: " << *iterator << std::endl;
// std::cout << "Updated cells: ";

// for (const std::string& updatedCellId : updateHistory_.top().ids()) {
//     std::cout << updatedCellId << " ";
// }

// std::cout << std::endl;

// for (const auto& cell : latinSquare.getGrid()) {
//     std::cout << cell.id() << ": number: " << cell.number() << ", entropy: " << cell.entropy()
//               << ", remaining numbers: ";

//     for (const int number : cell.numbers()) {
//         std::cout << number << " ";
//     }

//     std::cout << std::endl;
// }

// std::cout << std::endl;

// 2)
// std::cout << it << ". iteration (BACKTRACKING)" << std::endl;
// std::cout << "Previous cell: " << updateData.id() << " with number: " << number
//           << std::endl;

// for (const auto& cell : latinSquare.getGrid()) {
//     std::cout << cell.id() << ": number: " << cell.number() << ", entropy: " << cell.entropy()
//               << ", remaining numbers: ";

//     for (const int number : cell.numbers()) {
//         std::cout << number << " ";
//     }

//     std::cout << std::endl;
// }

// std::cout << std::endl;
