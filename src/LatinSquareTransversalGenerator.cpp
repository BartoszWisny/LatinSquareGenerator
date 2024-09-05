#include "LatinSquareTransversalGenerator.hpp"

#include <random>

#include "LatinSquareRegions.hpp"

namespace LatinSquareGenerator {
    const std::vector<std::reference_wrapper<Cell>> LatinSquareTransversalGenerator::findRandomTransversal(
        LatinSquare& latinSquare) {
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());
        std::vector<std::reference_wrapper<Cell>> transversal;
        const auto size = latinSquare.getSize();

        while (transversal.size() < size) {
            auto& region = latinSquare.getEnabledRegionWithMinimumEntropy();

            if (region.getEntropy() > 0) {
                const auto& cells = region.getEnabledCells();
                auto iterator = cells.cbegin();
                std::advance(iterator, mersenneTwister() % cells.size());
                auto& cell = (*iterator).get();

                transversal.emplace_back(*iterator); // S1)
                const auto disabledCellsIds = latinSquare.getDisabledCellsIds(cell); // S2), S3), S4)
                latinSquare.disableRelatedRegions(cell); // S5)

                // TODO: implement TransversalUpdateData structure
                updateHistory_.push(cell.getId(), relatedCellsIds); // S6) this is only concept


                // TODO
                // 1. add chosen cell to transversal
                // 2. deactivate chosen cell
                // 3. decrement entropy in row, column and 'numbers' region where chosen cell is located
                // (make use of id of chosen cell and ids of regions - maybe split id of chosen cell and then match?)
                // 4. get related available cells (available == true and same row, column or number)
                // 5. deactivate all these cells
                // 6. for every cell from these cells decrement entropy in row, column and 'numbers' region
                // 7. deactivate row, column and 'numbers' region where chosen cell is located
                // 8. save id of chosen cell and ids of other updated cells to update history
                // (must be separate to activate back row, column and 'numbers' region where chosen cell is located;
                //  create new structure for this case: std::string chosenCellId, std::set<std::string> updatedCellsIds)
                // probably can be done simpler:
                // 1. add chosen cell to transversal
                // 2. get all available related cells including chosen cell (available == true and same row, column or number)
                // 3. deactivate all these cells
                // 4. for every cell from these cells decrement entropy in row, column and 'numbers' region
                // 5. deactivate row, column and 'numbers' region where chosen cell is located
                // 6. save id of chosen cell and ids of other updated cells to update history
            } else { // backtracking
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
