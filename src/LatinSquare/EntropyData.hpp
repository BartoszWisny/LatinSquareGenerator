#pragma once

#include <bit>
#include <cstdint>
#include <vector>
#include <utility>

#include "Constants.hpp"

namespace LatinSquare {
    class EntropyData {
        public:
            EntropyData() = default;

            inline constexpr explicit EntropyData(const uint_fast8_t maxEntropy) noexcept
                : maxEntropy_(maxEntropy),
                  numbers_(maxEntropy_ < MAX_SIZE ? (1ULL << maxEntropy_) - 1 : 0xFFFFFFFFFFFFFFFF), bit_(0) {}

            EntropyData(const EntropyData&) = default;
            EntropyData& operator=(const EntropyData&) = default;

            inline constexpr EntropyData(EntropyData&& other) noexcept
                : maxEntropy_(other.maxEntropy_), numbers_(other.numbers_), bit_(other.bit_) {}

            inline constexpr EntropyData& operator=(EntropyData&& other) noexcept {
                if (this != &other) {
                    maxEntropy_ = std::exchange(other.maxEntropy_, 0);
                    numbers_ = std::exchange(other.numbers_, 0);
                    bit_ = std::exchange(other.bit_, 0);
                }

                return *this;
            }

            [[nodiscard]] inline constexpr uint_fast8_t entropy() const noexcept{
                return std::popcount(numbers_);
            }

            inline constexpr void clear() noexcept {
                numbers_ = 0;
            }

            [[nodiscard]] inline constexpr uint_fast64_t canBeRemoved(const uint_fast8_t number) noexcept {
                bit_ = 1ULL << number;
                return numbers_ & bit_;
            }

            inline constexpr void remove(const uint_fast8_t number) noexcept {
                bit_ = 1ULL << number;
                numbers_ &= ~bit_;
            }

            inline constexpr void remove() noexcept {
                numbers_ &= ~bit_;
            }

            inline constexpr void restore(const uint_fast8_t number) noexcept {
                bit_ = 1ULL << number;
                numbers_ |= bit_;
            }

            [[nodiscard]] const std::vector<uint_fast8_t> numbers() const noexcept;

        private:
            uint_fast8_t maxEntropy_;
            uint_fast64_t numbers_;
            uint_fast64_t bit_;
    };
}
