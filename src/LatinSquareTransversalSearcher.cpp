// #include "LatinSquareTransversalSearcher.hpp"

// #include <algorithm>
// #include <random>

// namespace LatinSquareGenerator {
//     std::vector<Cell> findRandomTransversal(LatinSquare latinSquare) {
//         std::random_device randomDevice;
//         std::mt19937 mersenneTwister(randomDevice());
//         const auto& grid = latinSquare.getGrid();
//         std::vector<std::reference_wrapper<Cell>> availableCells;
//         std::copy(grid.begin(), grid.end(), std::back_inserter(availableCells),

//         std::vector<std::reference_wrapper<Cell>> transversal;

//         while (transversal.size() < latinSquare.getSize()) {

//         }


//         std::vector<std::reference_wrapper<Cell>> updatedCells;
//         std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(updatedCells),
//                      [this, &filledCell](const auto& cell) { return checkIfRelatedCell(filledCell, cell); });
//         std::erase_if(updatedCells,
//                       [&filledCell](const auto& cellRef) {
//                           return !cellRef.get().removeRemainingNumber(filledCell.getNumber());
//                       });
//         std::set<std::string> updatedCellsIds;
//         std::transform(updatedCells.cbegin(), updatedCells.cend(),
//                        std::inserter(updatedCellsIds, updatedCellsIds.cend()),
//                        [](const auto& cellRef) { return cellRef.get().getId(); });

//         return updatedCellsIds;
//     }
// }
