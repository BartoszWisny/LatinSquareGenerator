#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <utility>

#include "Cell.hpp"

namespace LatinSquare {
    class Region {
        public:
            Region() = default;

            inline constexpr explicit Region(const uint_fast8_t index, const std::vector<std::shared_ptr<Cell>>& cells,
                const uint_fast8_t size) noexcept
                : index_(index), cells_(cells), size_(size), entropy_(size_), notEnabled_(false) {
                enabledCellIndexes_.reserve(size_);
            }

            Region(const Region&) = default;
            Region& operator=(const Region&) = default;

            inline constexpr Region(Region&& other) noexcept {
                index_ = std::exchange(other.index_, 0);
                cells_ = std::move(other.cells_);
                size_ = std::exchange(other.size_, 0);
                entropy_ = std::exchange(other.entropy_, 0);
                notEnabled_ = std::exchange(other.notEnabled_, true);
                enabledCellIndexes_ = std::move(other.enabledCellIndexes_);
            }

            inline constexpr Region& operator=(Region&& other) noexcept {
                if (this != &other) {
                    index_ = std::exchange(other.index_, 0);
                    cells_ = std::move(other.cells_);
                    size_ = std::exchange(other.size_, 0);
                    entropy_ = std::exchange(other.entropy_, 0);
                    notEnabled_ = std::exchange(other.notEnabled_, true);
                    enabledCellIndexes_ = std::move(other.enabledCellIndexes_);
                }

                return *this;
            }

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

            [[nodiscard]] const std::vector<uint_fast16_t>& enabledCellIndexes() noexcept;

        private:
            uint_fast8_t index_;
            std::vector<std::shared_ptr<Cell>> cells_;
            uint_fast8_t size_;
            uint_fast8_t entropy_;
            bool notEnabled_;
            std::vector<uint_fast16_t> enabledCellIndexes_;
    };
}
