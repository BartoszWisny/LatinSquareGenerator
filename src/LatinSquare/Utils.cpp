#include "Utils.hpp"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>

#include <cpp/string.hpp>

namespace LatinSquare {
    void printLatinSquareFullIds(LatinSquare& latinSquare) {
        latinSquare.sortGrid();
        const auto& grid = latinSquare.getGrid();

        std::string fullId;

        std::cout.put('\n');

        for (const auto& cell : grid) {
            fullId = cell.getFullId();

            std::cout.write(fullId.c_str(), fullId.size());
            std::cout.put('\n');
        }

        std::cout.put('\n');
    }

    void printLatinSquareBoard(LatinSquare& latinSquare) {
        latinSquare.sortGrid();
        const auto& grid = latinSquare.getGrid();
        const auto size = latinSquare.getSize();

        std::string leftBar = "+", repeatedLeftBar, spaces, numberString;
        auto number = 0;
        leftBar.append(std::string(static_cast<int>(std::log10(size) + 1) + 2, '-'));
        repeatedLeftBar = cpp::repeat(leftBar, size);

        std::cout.put('\n');

        for (const auto& cell : grid) {
            if (cell.getColumn() == 1) {
                std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                std::cout.put('+');
                std::cout.put('\n');
            }

            number = cell.getNumber();
            spaces = std::string(static_cast<int>(std::log10(size)) - static_cast<int>(std::log10(number)) + 1, ' ');
            numberString = std::to_string(number);

            std::cout.put('|');
            std::cout.write(spaces.c_str(), spaces.size());
            std::cout.write(numberString.c_str(), numberString.size());
            std::cout.put(' ');

            if (cell.getColumn() == size) {
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
