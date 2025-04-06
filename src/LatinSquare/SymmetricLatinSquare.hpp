#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <cpp/random.hpp>

#include "Cell.hpp"
#include "Constants.hpp"
#include "Region.hpp"
#include "Transversal/SymmetricCellUpdateData.hpp"
#include "TriangularRegion.hpp"

namespace LatinSquare {
    class SymmetricLatinSquare {
        public:
            explicit SymmetricLatinSquare(const uint_fast8_t size, const Type type) noexcept;
            explicit SymmetricLatinSquare(
                const uint_fast8_t size, const Type type, cpp::splitmix64& splitmix64) noexcept;

            explicit SymmetricLatinSquare(const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers) noexcept;
            explicit SymmetricLatinSquare(const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers,
                cpp::splitmix64& splitmix64) noexcept;

            SymmetricLatinSquare(const SymmetricLatinSquare&) = default;
            SymmetricLatinSquare& operator=(const SymmetricLatinSquare&) = default;

            SymmetricLatinSquare(SymmetricLatinSquare&&) noexcept = default;
            SymmetricLatinSquare& operator=(SymmetricLatinSquare&&) noexcept = default;

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

            inline constexpr void restore(
                const std::vector<uint_fast16_t>& indexes, const uint_fast8_t number) noexcept {
                for (const auto index : indexes) {
                    grid_[index]->restore(number);
                }
            }

            void set(const Type type) noexcept;
            void set(const std::vector<uint_fast8_t>& numbers) noexcept;
            void setNumberRegions() noexcept;

            [[nodiscard]] Cell& minEntropyCell() noexcept;
            [[nodiscard]] Cell& randomMinEntropyCell() noexcept;
            const std::vector<uint_fast16_t>& update(Cell& cell, const uint_fast8_t number) noexcept;
            [[nodiscard]] uint_fast8_t checkDiagonal() noexcept;
            void fillGrid() noexcept;
            void fillDiagonal() noexcept;
            void clearDiagonal() noexcept;

            [[nodiscard]] TriangularRegion& minEntropyTriangularRegion() noexcept;
            [[nodiscard]] TriangularRegion& randomMinEntropyTriangularRegion() noexcept;
            [[nodiscard]] const std::vector<Transversal::SymmetricCellUpdateData>& disable(
                const uint_fast16_t cellIndex, const uint_fast8_t regionIndex) noexcept;
            [[nodiscard]] const std::vector<Transversal::SymmetricCellUpdateData>& disableAndDecrease(
                const uint_fast16_t cellIndex, const uint_fast8_t regionIndex) noexcept;
            void enable(const uint_fast8_t regionIndex,
                const std::vector<Transversal::SymmetricCellUpdateData>& cellUpdateData) noexcept;
            void enableAndIncrease(
                const std::vector<Transversal::SymmetricCellUpdateData>& otherCellsUpdateData) noexcept;
            void enableAndIncrease(const Transversal::SymmetricCellUpdateData& updateData) noexcept;

        private:
            void reset() noexcept;
            void reset(const std::vector<uint_fast8_t>& numbers) noexcept;
            void resetNumberRegions() noexcept;

            uint_fast8_t size_;
            uint_fast16_t gridSize_;
            uint_fast16_t triangularGridSize_;
            uint_fast16_t entropyTriangularGridSize_;
            uint_fast8_t doubleSize_;
            uint_fast8_t maxUpdateSize_;
            uint_fast8_t maxFillDiagonalSize_;
            uint_fast8_t maxOtherCellsUpdateData_;
            std::vector<std::shared_ptr<Cell>> grid_;
            std::vector<std::shared_ptr<Cell>> triangularGrid_;
            std::vector<std::shared_ptr<Cell>> entropyTriangularGrid_;
            std::vector<std::shared_ptr<Cell>> diagonalGrid_;
            std::vector<TriangularRegion> triangularRegions_;
            std::vector<TriangularRegion> numberRegions_;
            cpp::splitmix64 splitmix64_;
            uint_fast16_t notFilled_;
            std::vector<uint_fast16_t> updateIndexes_;
            std::vector<uint_fast16_t> fillDiagonalIndexes_;
            std::vector<std::vector<std::shared_ptr<Cell>>> numberCells_;
            std::vector<Transversal::SymmetricCellUpdateData> cellUpdateData_;
            std::vector<Transversal::SymmetricCellUpdateData> otherCellsUpdateData_;
    };
}
