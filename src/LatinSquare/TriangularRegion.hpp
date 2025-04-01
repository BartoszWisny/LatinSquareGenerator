#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <utility>

#include "Cell.hpp"

namespace LatinSquare {
    class TriangularRegion {
        public:
            TriangularRegion() = default;

            inline constexpr explicit TriangularRegion(const uint_fast8_t index,
                const std::vector<std::shared_ptr<Cell>>& cells, const uint_fast8_t size) noexcept
                : index_(index), cells_(cells), size_(size), entropy_(size_), notEnabled_(false),
                  triangularEnabled_(2) {
                updatedCellIndexes_.reserve(size_);
                triangularGlobalEnabledCellIndexes_.reserve(cells.size());
                triangularLocalEnabledCellIndexes_.reserve(size_);
            }

            TriangularRegion(const TriangularRegion&) = default;
            TriangularRegion& operator=(const TriangularRegion&) = default;

            TriangularRegion(TriangularRegion&& other) noexcept {
                index_ = std::exchange(other.index_, 0);
                cells_ = std::move(other.cells_);
                size_ = std::exchange(other.size_, 0);
                entropy_ = std::exchange(other.entropy_, 0);
                notEnabled_ = std::exchange(other.notEnabled_, true);
                triangularEnabled_ = std::exchange(other.entropy_, 2);
                updatedCellIndexes_ = std::move(other.updatedCellIndexes_);
                triangularGlobalEnabledCellIndexes_ = std::move(other.triangularGlobalEnabledCellIndexes_);
                triangularLocalEnabledCellIndexes_ = std::move(other.triangularLocalEnabledCellIndexes_);
            }

            TriangularRegion& operator=(TriangularRegion&& other) noexcept {
                if (this != &other) {
                    index_ = std::exchange(other.index_, 0);
                    cells_ = std::move(other.cells_);
                    size_ = std::exchange(other.size_, 0);
                    entropy_ = std::exchange(other.entropy_, 0);
                    notEnabled_ = std::exchange(other.notEnabled_, true);
                    triangularEnabled_ = std::exchange(other.entropy_, 2);
                    updatedCellIndexes_ = std::move(other.updatedCellIndexes_);
                    triangularGlobalEnabledCellIndexes_ = std::move(other.triangularGlobalEnabledCellIndexes_);
                    triangularLocalEnabledCellIndexes_ = std::move(other.triangularLocalEnabledCellIndexes_);
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

            [[nodiscard]] inline constexpr uint_fast8_t triangularEnabled() const noexcept {
                return triangularEnabled_;
            }

            inline constexpr void enable() noexcept {
                notEnabled_ = false;
                ++triangularEnabled_;
            }

            inline constexpr void triangularEnable() noexcept {
                ++triangularEnabled_;
            }

            inline constexpr void triangularDisable() noexcept {
                --triangularEnabled_;
            }

            inline constexpr void decrease() noexcept {
                --entropy_;
            }

            inline constexpr void increase() noexcept {
                ++entropy_;
            }

            inline constexpr void disableAndDecrease() noexcept {
                notEnabled_ = true;
                --triangularEnabled_;
                --entropy_;
            }

            [[nodiscard]] const std::vector<uint_fast16_t>& updatedCellIndexes(
                const uint_fast8_t number) noexcept;
            [[nodiscard]] const std::vector<uint_fast16_t>& triangularGlobalEnabledCellIndexes() noexcept;
            [[nodiscard]] const std::vector<uint_fast16_t>& triangularLocalEnabledCellIndexes() noexcept;

        private:
            uint_fast8_t index_;
            std::vector<std::shared_ptr<Cell>> cells_;
            uint_fast8_t size_;
            uint_fast8_t entropy_;
            bool notEnabled_;
            uint_fast8_t triangularEnabled_;
            std::vector<uint_fast16_t> updatedCellIndexes_;
            std::vector<uint_fast16_t> triangularGlobalEnabledCellIndexes_;
            std::vector<uint_fast16_t> triangularLocalEnabledCellIndexes_;
    };
}
