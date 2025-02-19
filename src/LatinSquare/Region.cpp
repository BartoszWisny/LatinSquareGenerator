#include "Region.hpp"

namespace LatinSquare {
    const std::vector<uint_fast16_t> Region::enabledCellIndexes() const noexcept {
        std::vector<uint_fast16_t> indexes;
        indexes.reserve(size_);

        for (const auto& cell : cells_) {
            if (cell->enabled()) {
                indexes.emplace_back(cell->index());
            }
        }

        return indexes;
    }
}
