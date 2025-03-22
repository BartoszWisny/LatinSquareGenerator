#include "TriangularRegion.hpp"

namespace LatinSquare {
    const std::vector<uint_fast16_t> TriangularRegion::notFilledCellIndexes() const noexcept {
        std::vector<uint_fast16_t> indexes;
        indexes.reserve(size_);

        for (const auto& cell : cells_) {
            if (cell->notFilled()) {
                indexes.emplace_back(cell->index());
            }
        }

        return indexes;
    }
}
