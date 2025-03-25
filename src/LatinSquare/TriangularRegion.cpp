#include "TriangularRegion.hpp"

namespace LatinSquare {
    const std::vector<uint_fast16_t>& TriangularRegion::updatedCellIndexes(const uint_fast8_t number) noexcept {
        updatedCellIndexes_.clear();

        for (const auto& cell : cells_) {
            if (cell->notFilled() && cell->canBeRemoved(number)) {
                cell->remove();
                updatedCellIndexes_.emplace_back(cell->index());
            }
        }

        return updatedCellIndexes_;
    }
}
