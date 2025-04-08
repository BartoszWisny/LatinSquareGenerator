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

    uint_fast16_t Region::firstEnabledCellIndex() const noexcept {
        for (const auto& cell : cells_) {
            if (cell->enabled()) {
                return cell->index();
            }
        }

        return 0xFFFF;
    }
}
