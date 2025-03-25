#include "Region.hpp"

namespace LatinSquare {
    const std::vector<uint_fast16_t>& Region::enabledCellIndexes() noexcept {
        enabledCellIndexes_.resize(entropy_);
        uint_fast8_t counter = 0;

        for (const auto& cell : cells_) {
            if (cell->enabled()) {
                enabledCellIndexes_[counter] = cell->index();

                if (++counter == entropy_) {
                    break;
                }
            }
        }

        return enabledCellIndexes_;
    }
}
