#include "Utils.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <iterator>
#include <string>

namespace LatinSquare {
    void printLatinSquareFullIds(LatinSquare& latinSquare) {
        latinSquare.sortGrid();

        const auto& grid = latinSquare.getGrid();

        std::cout << std::endl;

        for (const auto& cell : grid) {
            std::cout << cell.getFullId() << std::endl;
        }

        std::cout << std::endl;
    }

    void printLatinSquareBoard(LatinSquare& latinSquare) {
        latinSquare.sortGrid();

        const auto& grid = latinSquare.getGrid();
        const auto size = latinSquare.getSize();

        const auto leftBar = std::format("+{}", std::string(int(log10(size) + 1) + 2, '-'));
        auto number = 0;
        std::string spaces = "";

        std::cout << std::endl;

        for (const auto& cell : grid) {
            if (cell.getColumn() == 1) {
                std::fill_n(std::ostream_iterator<std::string>(std::cout), size, leftBar);
                std::cout << "+" << std::endl;
            }

            number = cell.getNumber();
            spaces = std::string(int(log10(size)) - int(log10(number)) + 1, ' ');

            std::cout << "|" << spaces << number << " ";

            if (cell.getColumn() == size) {
                std::cout << "|" << std::endl;
            }
        }

        std::fill_n(std::ostream_iterator<std::string>(std::cout), size, leftBar);
        std::cout << "+" << std::endl;
        std::cout << std::endl;
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
