#pragma once

#include <cstdint>
#include <vector>

#include "SymmetricCellUpdateData.hpp"

namespace Transversal {
    class SymmetricBacktrackingData {
        public:
            inline constexpr explicit SymmetricBacktrackingData(const uint_fast8_t regionIndex,
                const std::vector<SymmetricCellUpdateData>& cellUpdateData) noexcept
                : regionIndex_(regionIndex), cellUpdateData_(cellUpdateData) {}

            SymmetricBacktrackingData(const SymmetricBacktrackingData&) = default;
            SymmetricBacktrackingData& operator=(const SymmetricBacktrackingData&) = default;

            SymmetricBacktrackingData(SymmetricBacktrackingData&&) noexcept = default;
            SymmetricBacktrackingData& operator=(SymmetricBacktrackingData&&) noexcept = default;

            [[nodiscard]] inline constexpr uint_fast8_t regionIndex() const noexcept {
                return regionIndex_;
            }

            [[nodiscard]] inline const std::vector<SymmetricCellUpdateData>& cellUpdateData() const noexcept {
                return cellUpdateData_;
            }

        private:
            uint_fast8_t regionIndex_;
            std::vector<SymmetricCellUpdateData> cellUpdateData_;
    };
}
