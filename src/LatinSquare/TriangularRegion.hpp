#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <utility>

#include "Cell.hpp"

namespace LatinSquare {
    class TriangularRegion {
        public:
            inline constexpr explicit TriangularRegion(const uint_fast8_t index,
                const std::vector<std::shared_ptr<Cell>> cells, const uint_fast8_t size) noexcept
                : index_(index), cells_(cells), size_(size) {}

            TriangularRegion(const TriangularRegion&) = default;
            TriangularRegion& operator=(const TriangularRegion&) = default;

            TriangularRegion(TriangularRegion&& other) noexcept {
                index_ = std::exchange(other.index_, 0);
                cells_ = std::move(other.cells_);
                size_ = std::exchange(other.size_, 0);
            }

            TriangularRegion& operator=(TriangularRegion&& other) noexcept {
                if (this != &other) {
                    index_ = std::exchange(other.index_, 0);
                    cells_ = std::move(other.cells_);
                    size_ = std::exchange(other.size_, 0);
                }

                return *this;
            }

            [[nodiscard]] inline constexpr uint_fast8_t index() const noexcept {
                return index_;
            }

            [[nodiscard]] const std::vector<uint_fast16_t> notFilledCellIndexes() const noexcept;

        private:
            uint_fast8_t index_;
            std::vector<std::shared_ptr<Cell>> cells_;
            uint_fast8_t size_;
    };
}
