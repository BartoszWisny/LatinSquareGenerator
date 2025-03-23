#pragma once

#include <cstdint>
#include <memory>
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

            explicit LatinSquare(const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers) noexcept;
            explicit LatinSquare(const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers,
                cpp::splitmix64& splitmix64) noexcept;

            LatinSquare(const LatinSquare& other)
                : size_(other.size_), gridSize_(other.gridSize_), entropyGridSize_(other.entropyGridSize_),
                  doubleSize_(other.doubleSize_), regionsSize_(other.regionsSize_), regions_(other.regions_),
                  splitmix64_(other.splitmix64_), notFilled_(other.notFilled_) {
                grid_.reserve(other.grid_.size());

                for (const auto& cell : other.grid_) {
                    grid_.emplace_back(std::make_shared<Cell>(*cell));
                }

                entropyGrid_.reserve(other.entropyGrid_.size());

                for (const auto& cell : other.entropyGrid_) {
                    entropyGrid_.emplace_back(std::make_shared<Cell>(*cell));
                }
            }

            LatinSquare& operator=(const LatinSquare& other) {
                if (this != &other) {
                    size_ = other.size_;
                    gridSize_ = other.gridSize_;
                    entropyGridSize_ = other.entropyGridSize_;
                    doubleSize_ = other.doubleSize_;
                    regionsSize_ = other.regionsSize_;
                    regions_ = other.regions_;
                    splitmix64_ = other.splitmix64_;
                    notFilled_ = other.notFilled_;
                    grid_.clear();
                    grid_.reserve(other.grid_.size());

                    for (const auto& cell : other.grid_) {
                        grid_.emplace_back(std::make_shared<Cell>(*cell));
                    }

                    entropyGrid_.clear();
                    entropyGrid_.reserve(other.entropyGrid_.size());

                    for (const auto& cell : other.entropyGrid_) {
                        entropyGrid_.emplace_back(std::make_shared<Cell>(*cell));
                    }
                }

                return *this;
            }

            [[nodiscard]] inline constexpr uint_fast8_t size() const noexcept {
                return size_;
            }

            [[nodiscard]] inline const std::vector<std::shared_ptr<Cell>>& grid() const noexcept {
                return grid_;
            }

            [[nodiscard]] inline constexpr uint_fast16_t notFilled() const noexcept {
                return notFilled_;
            }

            inline void fillAndClear(Cell& cell, const uint_fast8_t number) noexcept {
                cell.fillAndClear(number);
                --notFilled_;
            }

            inline void set(const uint_fast16_t index, const EntropyData& entropyData) noexcept {
                grid_[index]->set(entropyData);
            }

            inline void clearAndRemove(const uint_fast16_t index, const EntropyData& entropyData) noexcept {
                grid_[index]->clearAndRemove(entropyData);
                ++notFilled_;
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

            void set(const Type type) noexcept;
            void set(const std::vector<uint_fast8_t>& numbers) noexcept;
            void setRegions() noexcept;

            [[nodiscard]] Cell& minEntropyCell() noexcept;
            [[nodiscard]] Cell& randomMinEntropyCell() noexcept;
            const std::vector<uint_fast16_t> update(Cell& cell, const uint_fast8_t number) noexcept;

            [[nodiscard]] Region& minEntropyRegion() noexcept;
            [[nodiscard]] Region& randomMinEntropyRegion() noexcept;
            void disable(const uint_fast16_t index) noexcept;
            [[nodiscard]] const std::vector<uint_fast16_t> disableAndDecrease(const uint_fast16_t index) noexcept;
            void enable(const uint_fast16_t index) noexcept;
            void enableAndIncrease(const std::vector<uint_fast16_t>& indexes) noexcept;

        private:
            void reset() noexcept;
            void reset(const std::vector<uint_fast8_t>& numbers) noexcept;
            void resetRegions() noexcept;

            uint_fast8_t size_;
            uint_fast16_t gridSize_;
            uint_fast16_t entropyGridSize_;
            uint_fast8_t doubleSize_;
            uint_fast8_t regionsSize_;
            std::vector<std::shared_ptr<Cell>> grid_;
            std::vector<std::shared_ptr<Cell>> entropyGrid_;
            std::vector<Region> regions_;
            cpp::splitmix64 splitmix64_;
            uint_fast16_t notFilled_;
    };
}
