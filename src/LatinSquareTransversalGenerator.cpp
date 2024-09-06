#include "LatinSquareTransversalGenerator.hpp"

#include <iterator>
#include <random>

// #include <iostream>
// #include <algorithm>

namespace LatinSquareGenerator {
    const std::vector<std::reference_wrapper<Cell>> LatinSquareTransversalGenerator::findRandomTransversal(
        LatinSquare& latinSquare) {
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());
        latinSquare.setRegions(); // TODO: consider if this is good place for this function call
        std::vector<std::reference_wrapper<Cell>> transversal;
        const auto size = latinSquare.getSize();

        // for (auto region : latinSquare.getRegions()) {
        //     std::cout << region.getId() << ": entropy: " << region.getEntropy() << ", cells: ";
        //     for (auto cell : region.getCells()) {
        //         std::cout << cell.get().getFullId() << " ";
        //     }
        //     std::cout << ", enabled: " << region.isEnabled() << std::endl;
        // }

        // std::cout << std::endl;

        while (transversal.size() < (unsigned) size) {
            auto& region = latinSquare.getEnabledRegionWithMinimumEntropy();

            if (region.getEntropy() > 0) {
                const auto cells = region.getEnabledCells();
                auto iterator = cells.cbegin();
                std::advance(iterator, mersenneTwister() % cells.size());
                auto& cell = (*iterator).get();

                transversal.emplace_back(*iterator);
                const auto disabledCellsIds = latinSquare.getDisabledCellsIds(cell);
                latinSquare.disableRelatedRegions(cell);
                updateHistory_.push(TransversalUpdateData(cell, disabledCellsIds));

                // std::cout << transversal.size() << ". iteration" << std::endl;
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
                //                    std::inserter(enabledCellsIds, enabledCellsIds.cend()),
                //                    [](const auto& cellRef) { return cellRef.get().getFullId(); });

                //     for (auto cellId : enabledCellsIds) {
                //         std::cout << cellId << " ";
                //     }

                //     std::cout << ", enabled: " << region.isEnabled() << std::endl;
                // }

                // std::cout << std::endl;
            } else { // backtracking
                break; // to be implemented

                // TODO
                // 1. remove last added cell from transversal (use pop_back)
                // 2. get ids of cells from update history
                // 3. remove entry from update history
                // 4. get all cells with ids from update history
                // 5. activate back all these cells
                // 6. for every cell from these cells increment entropy in row, column and 'numbers' region
                // 7. activate back row, column and 'numbers' region where chosen cell is located
            }
        }

        return transversal;
    }
}
