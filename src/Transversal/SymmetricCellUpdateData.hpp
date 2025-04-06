#pragma once

#include <cstdint>

namespace Transversal {
    class SymmetricCellUpdateData {
        public:
            inline constexpr explicit SymmetricCellUpdateData(
                const uint_fast16_t cellIndex, const uint_fast8_t regionIndex) noexcept
                : cellIndex_(cellIndex), regionIndex_(regionIndex) {}

            inline constexpr explicit SymmetricCellUpdateData(
                const uint_fast16_t cellIndex, const uint_fast8_t regionIndex, const bool enabled) noexcept
                : cellIndex_(cellIndex), regionIndex_(regionIndex), enabled_(enabled) {}

            // SymmetricCellUpdateData(const SymmetricCellUpdateData& other)
            //     : cellIndex_(other.cellIndex_), regionIndex_(other.regionIndex_), enabled_(other.enabled_) {}

            // SymmetricCellUpdateData& operator=(const SymmetricCellUpdateData& other) {
            //     if (this != &other) {
            //         cellIndex_ = other.cellIndex_;
            //         regionIndex_ = other.regionIndex_;
            //         enabled_ = other.enabled_;
            //     }

            //     return *this;
            // }

            // SymmetricCellUpdateData(SymmetricCellUpdateData&& other) noexcept {
            //     cellIndex_ = std::exchange(other.cellIndex_, 0);
            //     regionIndex_ = std::exchange(other.regionIndex_, 0);
            //     enabled_ = std::exchange(other.enabled_, false);
            // }

            // SymmetricCellUpdateData& operator=(SymmetricCellUpdateData&& other) noexcept {
            //     if (this != &other) {
            //         cellIndex_ = std::exchange(other.cellIndex_, 0);
            //         regionIndex_ = std::exchange(other.regionIndex_, 0);
            //         enabled_ = std::exchange(other.enabled_, false);
            //     }

            //     return *this;
            // }

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

            [[nodiscard]] inline constexpr bool enabled() const noexcept {
                return enabled_;
            }

        private:
            uint_fast16_t cellIndex_;
            uint_fast8_t regionIndex_;
            bool enabled_;
    };
}
