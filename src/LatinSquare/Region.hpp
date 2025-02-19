#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Cell.hpp"

namespace LatinSquare {
    class Region {
        public:
            Region() = default;

            inline constexpr explicit Region(const uint_fast8_t index, const std::vector<std::shared_ptr<Cell>> cells,
                const uint_fast8_t size) noexcept
                : index_(index), cells_(cells), size_(size), entropy_(size_), notEnabled_(false) {}

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

            inline constexpr void disable() noexcept {
                notEnabled_ = true;
            }

            inline constexpr void decrease() noexcept {
                --entropy_;
            }

            inline constexpr void increase() noexcept {
                ++entropy_;
            }

            [[nodiscard]] const std::vector<uint_fast16_t> enabledCellIndexes() const noexcept;

        private:
            uint_fast8_t index_;
            std::vector<std::shared_ptr<Cell>> cells_;
            uint_fast8_t size_;
            uint_fast8_t entropy_;
            bool notEnabled_;
    };
}
