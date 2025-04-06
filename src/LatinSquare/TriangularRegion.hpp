#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Cell.hpp"

namespace LatinSquare {
    class TriangularRegion {
        public:
            TriangularRegion() = default;

            inline constexpr explicit TriangularRegion(const uint_fast8_t index,
                const std::vector<std::shared_ptr<Cell>>& cells, const uint_fast8_t size) noexcept
                : index_(index), cells_(cells), size_(size), entropy_(cells.size()), notEnabled_(false), counter_(0) {
                updatedCellIndexes_.reserve(size_);
                triangularGlobalEnabledCellIndexes_.reserve(size_);
                triangularLocalEnabledCellIndexes_.reserve(size_);
                triangularOtherLocalEnabledCellIndexes_.reserve(size_);
            }

            TriangularRegion(const TriangularRegion&) = default;
            TriangularRegion& operator=(const TriangularRegion&) = default;

            TriangularRegion(TriangularRegion&&) noexcept = default;
            TriangularRegion& operator=(TriangularRegion&&) noexcept = default;

            [[nodiscard]] inline constexpr uint_fast8_t index() const noexcept {
                return index_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t entropy() const noexcept {
                return entropy_;
            }

            [[nodiscard]] inline constexpr bool notEnabled() const noexcept {
                return notEnabled_;
            }

            inline constexpr void enable() noexcept {
                notEnabled_ = false;
            }

            inline constexpr void decrease() noexcept {
                --entropy_;
            }

            inline constexpr void increase() noexcept {
                ++entropy_;
            }

            inline constexpr void disableAndDecrease() noexcept {
                notEnabled_ = true;
                --entropy_;
            }

            [[nodiscard]] const std::vector<uint_fast16_t>& updatedCellIndexes(const uint_fast8_t number) noexcept;
            [[nodiscard]] const std::vector<uint_fast16_t>& triangularGlobalEnabledCellIndexes() noexcept;
            [[nodiscard]] const std::vector<uint_fast16_t>& triangularLocalEnabledCellIndexes() noexcept;
            [[nodiscard]] const std::vector<uint_fast16_t>& triangularOtherLocalEnabledCellIndexes() noexcept;

        private:
            uint_fast8_t index_;
            std::vector<std::shared_ptr<Cell>> cells_;
            uint_fast8_t size_;
            uint_fast8_t entropy_;
            bool notEnabled_;
            uint_fast8_t counter_;
            std::vector<uint_fast16_t> updatedCellIndexes_;
            std::vector<uint_fast16_t> triangularGlobalEnabledCellIndexes_;
            std::vector<uint_fast16_t> triangularLocalEnabledCellIndexes_;
            std::vector<uint_fast16_t> triangularOtherLocalEnabledCellIndexes_;
    };
}
