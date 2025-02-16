#pragma once

#include <cstdint>

namespace Transversal {
    class BacktrackingData {
        public:
            inline constexpr explicit BacktrackingData(
                const uint_fast8_t regionIndex, const uint_fast16_t cellIndex) noexcept
                : regionIndex_(regionIndex), cellIndex_(cellIndex) {}

            [[nodiscard]] inline constexpr uint_fast8_t regionIndex() const noexcept {
                return regionIndex_;
            }

            [[nodiscard]] inline constexpr uint_fast16_t cellIndex() const noexcept {
                return cellIndex_;
            }

        private:
            uint_fast8_t regionIndex_;
            uint_fast16_t cellIndex_;
    };
}
