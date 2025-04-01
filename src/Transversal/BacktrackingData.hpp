#pragma once

#include <cstdint>
#include <utility>

namespace Transversal {
    class BacktrackingData {
        public:
            inline constexpr explicit BacktrackingData(
                const uint_fast8_t regionIndex, const uint_fast16_t cellIndex) noexcept
                : regionIndex_(regionIndex), cellIndex_(cellIndex) {}

            BacktrackingData(const BacktrackingData& other)
                : regionIndex_(other.regionIndex_), cellIndex_(other.cellIndex_) {}

            BacktrackingData& operator=(const BacktrackingData& other) {
                if (this != &other) {
                    regionIndex_ = other.regionIndex_;
                    cellIndex_ = other.cellIndex_;
                }

                return *this;
            }

            BacktrackingData(BacktrackingData&& other) noexcept {
                regionIndex_ = std::exchange(other.regionIndex_, 0);
                cellIndex_ = std::exchange(other.cellIndex_, 0);
            }

            BacktrackingData& operator=(BacktrackingData&& other) noexcept {
                if (this != &other) {
                    regionIndex_ = std::exchange(other.regionIndex_, 0);
                    cellIndex_ = std::exchange(other.cellIndex_, 0);
                }

                return *this;
            }

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
