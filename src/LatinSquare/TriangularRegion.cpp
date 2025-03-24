#include "TriangularRegion.hpp"

namespace LatinSquare {
    const std::vector<uint_fast16_t> TriangularRegion::updatedCellIndexes(const uint_fast8_t number) const noexcept {
        std::vector<uint_fast16_t> indexes;
        indexes.reserve(size_);

        for (const auto& cell : cells_) {
            if (cell->notFilled() && cell->remove(number)) {
                indexes.emplace_back(cell->index());
            }
        }

        return indexes;
    }
}
