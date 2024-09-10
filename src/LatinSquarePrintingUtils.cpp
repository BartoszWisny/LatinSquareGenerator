#include "LatinSquarePrintingUtils.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <iterator>

#include "LatinSquare.hpp"

namespace LatinSquareGenerator {
    void printLatinSquareFullIds(LatinSquare& latinSquare) {
        latinSquare.sortGridByRows();
        const auto& grid = latinSquare.getGrid();

        for (const auto& cell : grid) {
            std::cout << cell.getFullId() << std::endl;
        }

        std::cout << std::endl;
    }

    void printLatinSquareBoard(LatinSquare& latinSquare) {
        latinSquare.sortGridByRows();
        const auto& grid = latinSquare.getGrid();
        const auto size = latinSquare.getSize();
        const int gridSize = grid.size();
        const auto horizontalBar = std::format("+{}", std::string(int(log10(size) + 1) + 2, '-'));
        auto number = 0;
        std::string spaces = "";

        for (int index = 0; index < gridSize; ++index) {
            if (index % size == 0) {
                std::fill_n(std::ostream_iterator<std::string>(std::cout), size, horizontalBar);
                std::cout << "+" << std::endl;
            }

            number = grid.at(index).getNumber();
            spaces = std::string(int(log10(size)) - int(log10(number)) + 1, ' ');

            std::cout << "|" << spaces << grid.at(index).getNumber() << " ";

            if (index % size == size - 1) {
                std::cout << "|" << std::endl;
            }
        }

        std::fill_n(std::ostream_iterator<std::string>(std::cout), size, horizontalBar);
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
            std::cout << "Transversal was not found" << std::endl;
        }

        sortTransversal(transversal);

        for (const auto& cellRef : transversal) {
            std::cout << cellRef.get().getFullId() << std::endl;
        }

        std::cout << std::endl;
    }

    // TODO: add function for printing transversal visually
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
