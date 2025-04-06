#pragma once

#include <cstdint>

namespace Transversal {
    class SymmetricCellUpdateData {
        public:
            inline constexpr explicit SymmetricCellUpdateData(
                const uint_fast16_t cellIndex, const uint_fast8_t regionIndex) noexcept
                : cellIndex_(cellIndex), regionIndex_(regionIndex) {}

            SymmetricCellUpdateData(const SymmetricCellUpdateData&) = default;
            SymmetricCellUpdateData& operator=(const SymmetricCellUpdateData&) = default;

            SymmetricCellUpdateData(SymmetricCellUpdateData&&) noexcept = default;
            SymmetricCellUpdateData& operator=(SymmetricCellUpdateData&&) noexcept = default;

            [[nodiscard]] inline constexpr uint_fast16_t cellIndex() const noexcept {
                return cellIndex_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t regionIndex() const noexcept {
                return regionIndex_;
            }

        private:
            uint_fast16_t cellIndex_;
            uint_fast8_t regionIndex_;
    };
}
