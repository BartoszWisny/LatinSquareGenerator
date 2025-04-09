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

    // const std::vector<uint_fast16_t>& TriangularRegion::triangularLocalEnabledCellIndexes() noexcept {
    //     triangularLocalEnabledCellIndexes_.resize(entropy_);
    //     counter_ = 0;

    //     for (const auto& cell : cells_) {
    //         if (cell->triangularEnabled(index_)) {
    //             triangularLocalEnabledCellIndexes_[counter_] = cell->index();

    //             if (++counter_ == entropy_) {
    //                 break;
    //             }
    //         }
    //     }

    //     return triangularLocalEnabledCellIndexes_;
    // }

    uint_fast16_t TriangularRegion::firstTriangularLocalEnabledCellIndex() noexcept {
        for (const auto& cell : cells_) {
            if (cell->triangularEnabled(index_)) {
                return cell->index();
            }
        }

        return cells_[0]->index();
    }

    const std::vector<uint_fast16_t>& TriangularRegion::triangularOtherLocalEnabledCellIndexes() noexcept {
        triangularOtherLocalEnabledCellIndexes_.clear();

        for (const auto& cell : cells_) {
            if (cell->otherTriangularEnabled(index_)) {
                triangularOtherLocalEnabledCellIndexes_.emplace_back(cell->index());
            }
        }

        return triangularOtherLocalEnabledCellIndexes_;
    }
}
