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

    const std::vector<uint_fast16_t>& TriangularRegion::triangularGlobalEnabledCellIndexes() noexcept {
        triangularGlobalEnabledCellIndexes_.clear();

        for (const auto& cell : cells_) {
            if (cell->triangularEnabled()) {
                triangularGlobalEnabledCellIndexes_.emplace_back(cell->index());
            }
        }

        return triangularGlobalEnabledCellIndexes_;
    }

    const std::vector<uint_fast16_t>& TriangularRegion::triangularLocalEnabledCellIndexes() noexcept {
        triangularLocalEnabledCellIndexes_.resize(entropy_);
        uint_fast8_t counter = 0;

        for (const auto& cell : cells_) {
            if (cell->triangularEnabled(index_)) {
                triangularLocalEnabledCellIndexes_[counter] = cell->index();

                if (++counter == entropy_) {
                    break;
                }
            }
        }

        return triangularLocalEnabledCellIndexes_;
    }
}
