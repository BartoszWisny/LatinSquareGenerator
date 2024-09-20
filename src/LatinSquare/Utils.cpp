#include "Utils.hpp"

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

#include <cpp/string.hpp>

namespace LatinSquare {
    void printLatinSquareFullIds(LatinSquare& latinSquare) {
        latinSquare.sortGrid();
        const auto& grid = latinSquare.getGrid();

        std::ostringstream ostringstream;
        ostringstream << '\n';

        for (const auto& cell : grid) {
            ostringstream << cell.getFullId() << '\n';
        }

        ostringstream << '\n';
        std::cout << ostringstream.str();
    }

    void printLatinSquareBoard(LatinSquare& latinSquare) {
        latinSquare.sortGrid();
        const auto& grid = latinSquare.getGrid();
        const auto size = latinSquare.getSize();

        std::string leftBar = "+";
        leftBar.append(std::string(static_cast<int>(std::log10(size) + 1) + 2, '-'));
        auto number = 0;
        std::string spaces = "";

        std::ostringstream ostringstream;
        ostringstream << '\n';

        for (const auto& cell : grid) {
            if (cell.getColumn() == 1) {
                ostringstream << cpp::repeat(leftBar, size) << "+\n";
            }

            number = cell.getNumber();
            spaces = std::string(static_cast<int>(std::log10(size)) - static_cast<int>(std::log10(number)) + 1, ' ');

            ostringstream << '|' << spaces << number << ' ';

            if (cell.getColumn() == size) {
                ostringstream << "|\n";
            }
        }

        ostringstream << cpp::repeat(leftBar, size) << "+\n\n";
        std::cout << ostringstream.str();
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
