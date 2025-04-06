#include "Region.hpp"

namespace LatinSquare {
    const std::vector<uint_fast16_t>& Region::enabledCellIndexes() noexcept {
        enabledCellIndexes_.resize(entropy_);
        counter_ = 0;

        for (const auto& cell : cells_) {
            if (cell->enabled()) {
                enabledCellIndexes_[counter_] = cell->index();

                if (++counter_ == entropy_) {
                    break;
                }
            }
        }

        return enabledCellIndexes_;
    }
}
