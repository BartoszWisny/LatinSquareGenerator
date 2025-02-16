#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include <cpp/random.hpp>

#include "Cell.hpp"
#include "Constants.hpp"
#include "Region.hpp"

namespace LatinSquare {
    class LatinSquare {
        public:
            explicit LatinSquare(const uint_fast8_t size, const Type type) noexcept;
            explicit LatinSquare(const uint_fast8_t size, const Type type, cpp::splitmix64& splitmix64) noexcept;

            [[nodiscard]] inline constexpr uint_fast8_t size() const noexcept {
                return size_;
            }

            [[nodiscard]] inline const std::vector<std::shared_ptr<Cell>>& grid() const noexcept {
                return grid_;
            }

            [[nodiscard]] inline constexpr uint_fast16_t notFilled() const noexcept {
                return notFilled_;
            }

            inline void fill(const uint_fast16_t index, const uint_fast8_t number) noexcept {
                grid_[index]->fill(number);
                --notFilled_;
            }

            inline void set(const uint_fast16_t index, const EntropyData& entropyData) noexcept {
                grid_[index]->set(entropyData);
            }

            inline void clear(const uint_fast16_t index, const EntropyData& entropyData) noexcept {
                grid_[index]->clear(entropyData);
                ++notFilled_;
            }

            inline constexpr void restore(
                const std::vector<uint_fast16_t>& indexes, const uint_fast8_t number) noexcept {
                for (const auto index : indexes) {
                    grid_[index]->restore(number);
                }
            }

            inline constexpr void disable(const std::vector<uint_fast16_t>& indexes) noexcept {
                for (const auto index : indexes) {
                    grid_[index]->disable();
                }
            }

            inline constexpr void enable(const std::vector<uint_fast16_t>& indexes) noexcept {
                for (const auto index : indexes) {
                    grid_[index]->enable();
                }
            }

            void setRegions() noexcept;

            [[nodiscard]] std::shared_ptr<Cell> minEntropyCell() noexcept;
            [[nodiscard]] std::shared_ptr<Cell> randomMinEntropyCell() noexcept;
            [[nodiscard]] const std::vector<uint_fast16_t> relatedToFilledCellIndexes(
                const std::shared_ptr<Cell>  filledCell) const noexcept;
            void update(std::vector<uint_fast16_t>& indexes, const uint_fast8_t number) noexcept;

            [[nodiscard]] Region& minEntropyRegion() noexcept;
            [[nodiscard]] Region& randomMinEntropyRegion() noexcept;

            [[nodiscard]] const std::vector<uint_fast16_t> relatedToChosenCellIndexes(
                const std::shared_ptr<Cell> chosenCell) const noexcept;
            void decrease(std::vector<uint_fast16_t>& indexes, const uint_fast16_t chosenIndex) noexcept;
            void disable(const uint_fast16_t index) noexcept;
            void increase(const std::vector<uint_fast16_t>& indexes) noexcept;
            void enable(const uint_fast16_t index) noexcept;

        private:
            void reset(const Type type) noexcept;

            uint_fast8_t size_;
            std::vector<std::shared_ptr<Cell>> grid_;
            std::vector<Region> regions_;
            cpp::splitmix64 splitmix64_;
            uint_fast16_t notFilled_;
    };
}
