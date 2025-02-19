#pragma once

#include <bit>
#include <cstdint>
#include <vector>

namespace LatinSquare {
    class EntropyData {
        public:
            EntropyData() = default;

            inline constexpr explicit EntropyData(const uint_fast8_t maxEntropy) noexcept
                : maxEntropy_(maxEntropy), numbers_((1ULL << maxEntropy_) - 1) {}

            [[nodiscard]] inline constexpr uint_fast8_t entropy() const noexcept{
                return std::popcount(numbers_);
            }

            inline constexpr void clear() noexcept {
                numbers_ = 0;
            }

            inline constexpr uint_fast64_t remove(const uint_fast8_t number) noexcept {
                const auto bit = 1ULL << number;
                const auto set = numbers_ & bit;
                numbers_ &= ~bit;
                return set;
            }

            inline constexpr void restore(const uint_fast8_t number) noexcept {
                numbers_ |= (1ULL << number);
            }

            [[nodiscard]] const std::vector<uint_fast8_t> numbers() const noexcept;

        private:
            uint_fast8_t maxEntropy_;
            uint_fast64_t numbers_;
    };
}
