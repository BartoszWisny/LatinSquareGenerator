#pragma once

#include <cstdint>

#include "EntropyData.hpp"

namespace LatinSquare {
    class BacktrackingData {
        public:
            inline constexpr explicit BacktrackingData(
                const uint_fast16_t index, const EntropyData& entropyData) noexcept
                : index_(index), entropyData_(entropyData) {}

            BacktrackingData(const BacktrackingData&) = default;
            BacktrackingData& operator=(const BacktrackingData&) = default;

            BacktrackingData(BacktrackingData&&) noexcept = default;
            BacktrackingData& operator=(BacktrackingData&&) noexcept = default;

            [[nodiscard]] inline constexpr uint_fast16_t index() const noexcept {
                return index_;
            }

            [[nodiscard]] inline const EntropyData& entropyData() const noexcept {
                return entropyData_;
            }

        private:
            uint_fast16_t index_;
            EntropyData entropyData_;
    };
}
