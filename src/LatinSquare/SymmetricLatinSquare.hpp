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

            SymmetricLatinSquare(const SymmetricLatinSquare& other)
                : size_(other.size_), gridSize_(other.gridSize_), triangularGridSize_(other.triangularGridSize_),
                  entropyTriangularGridSize_(other.entropyTriangularGridSize_), doubleSize_(other.doubleSize_),
                  maxUpdateSize_(other.maxUpdateSize_), regionsSize_(other.regionsSize_),
                  maxFillDiagonalSize_(other.maxFillDiagonalSize_),
                  maxDisableAndDecreaseSize_(other.maxDisableAndDecreaseSize_),
                  maxOtherCellsUpdateData_(other.maxOtherCellsUpdateData_), regions_(other.regions_),
                  triangularRegions_(other.triangularRegions_), numberRegions_(other.numberRegions_),
                  splitmix64_(other.splitmix64_), minCell_(other.minCell_), minRegion_(other.minRegion_),
                  minTriangularRegion_(other.minTriangularRegion_), minEntropy_(other.minEntropy_),
                  notFilled_(other.notFilled_), updateIndexes_(other.updateIndexes_),
                  fillDiagonalIndexes_(other.fillDiagonalIndexes_),
                  disableAndDecreaseIndexes_(other.disableAndDecreaseIndexes_), cellUpdateData_(other.cellUpdateData_),
                  otherCellsUpdateData_(other.otherCellsUpdateData_) {
                grid_.reserve(other.grid_.size());

                for (const auto& cell : other.grid_) {
                    grid_.emplace_back(std::make_shared<Cell>(*cell));
                }

                triangularGrid_.reserve(other.triangularGrid_.size());

                for (const auto& cell : other.triangularGrid_) {
                    triangularGrid_.emplace_back(std::make_shared<Cell>(*cell));
                }

                entropyTriangularGrid_.reserve(other.entropyTriangularGrid_.size());

                for (const auto& cell : other.entropyTriangularGrid_) {
                    entropyTriangularGrid_.emplace_back(std::make_shared<Cell>(*cell));
                }

                diagonalGrid_.reserve(other.diagonalGrid_.size());

                for (const auto& cell : other.diagonalGrid_) {
                    diagonalGrid_.emplace_back(std::make_shared<Cell>(*cell));
                }

                numberCells_.resize(other.numberCells_.size());

                for (size_t index = 0; index < other.numberCells_.size(); ++index) {
                    numberCells_[index].reserve(other.numberCells_[index].size());

                    for (const auto& cell : other.numberCells_[index]) {
                        numberCells_[index].emplace_back(std::make_shared<Cell>(*cell));
                    }
                }
            }

            SymmetricLatinSquare& operator=(const SymmetricLatinSquare& other) {
                if (this != &other) {
                    size_ = other.size_;
                    gridSize_ = other.gridSize_;
                    triangularGridSize_ = other.triangularGridSize_;
                    entropyTriangularGridSize_ = other.entropyTriangularGridSize_;
                    doubleSize_ = other.doubleSize_;
                    maxUpdateSize_ = other.maxUpdateSize_;
                    regionsSize_ = other.regionsSize_;
                    maxFillDiagonalSize_ = other.maxFillDiagonalSize_;
                    maxDisableAndDecreaseSize_ = other.maxDisableAndDecreaseSize_;
                    maxOtherCellsUpdateData_ = other.maxOtherCellsUpdateData_;
                    regions_ = other.regions_;
                    triangularRegions_ = other.triangularRegions_;
                    numberRegions_ = other.numberRegions_;
                    splitmix64_ = other.splitmix64_;
                    minCell_ = other.minCell_;
                    minRegion_ = other.minRegion_;
                    minTriangularRegion_ = other.minTriangularRegion_;
                    minEntropy_ = other.minEntropy_;
                    notFilled_ = other.notFilled_;
                    updateIndexes_ = other.updateIndexes_;
                    fillDiagonalIndexes_ = other.fillDiagonalIndexes_;
                    disableAndDecreaseIndexes_ = other.disableAndDecreaseIndexes_;
                    cellUpdateData_ = other.cellUpdateData_;
                    otherCellsUpdateData_ = other.otherCellsUpdateData_;
                    grid_.clear();
                    grid_.reserve(other.grid_.size());

                    for (const auto& cell : other.grid_) {
                        grid_.emplace_back(std::make_shared<Cell>(*cell));
                    }

                    triangularGrid_.clear();
                    triangularGrid_.reserve(other.triangularGrid_.size());

                    for (const auto& cell : other.triangularGrid_) {
                        triangularGrid_.emplace_back(std::make_shared<Cell>(*cell));
                    }

                    entropyTriangularGrid_.clear();
                    entropyTriangularGrid_.reserve(other.entropyTriangularGrid_.size());

                    for (const auto& cell : other.entropyTriangularGrid_) {
                        entropyTriangularGrid_.emplace_back(std::make_shared<Cell>(*cell));
                    }

                    diagonalGrid_.clear();
                    diagonalGrid_.reserve(other.diagonalGrid_.size());

                    for (const auto& cell : other.diagonalGrid_) {
                        diagonalGrid_.emplace_back(std::make_shared<Cell>(*cell));
                    }

                    numberCells_.clear();
                    numberCells_.resize(other.numberCells_.size());

                    for (size_t index = 0; index < other.numberCells_.size(); ++index) {
                        numberCells_[index].reserve(other.numberCells_[index].size());

                        for (const auto& cell : other.numberCells_[index]) {
                            numberCells_[index].emplace_back(std::make_shared<Cell>(*cell));
                        }
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

            [[nodiscard]] inline const std::vector<std::shared_ptr<Cell>>& triangularGrid() const noexcept {
                return triangularGrid_;
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
            void setRegions() noexcept;
            void setNumberRegions() noexcept;

            [[nodiscard]] Cell& minEntropyCell(const uint_fast16_t index) noexcept;
            [[nodiscard]] Cell& randomMinEntropyCell(const uint_fast16_t index) noexcept;
            const std::vector<uint_fast16_t>& update(Cell& cell, const uint_fast8_t number) noexcept;
            [[nodiscard]] uint_fast8_t checkDiagonal() noexcept;
            void fillGrid() noexcept;
            void fillDiagonal() noexcept;
            void clearDiagonal() noexcept;

            [[nodiscard]] Region& minEntropyRegion(const uint_fast8_t index) noexcept;
            [[nodiscard]] Region& lastNotChosenRegion() noexcept;
            [[nodiscard]] Region& randomMinEntropyRegion(const uint_fast8_t index) noexcept;
            void disable(const uint_fast16_t index) noexcept;
            [[nodiscard]] const std::vector<uint_fast16_t>& disableAndDecrease(const uint_fast16_t index) noexcept;
            void enable(const uint_fast16_t index) noexcept;
            void enableAndIncrease(const uint_fast16_t index) noexcept;
            void enableAndIncrease(const std::vector<uint_fast16_t>& indexes) noexcept;

            [[nodiscard]] TriangularRegion& minEntropyTriangularRegion(const uint_fast8_t index) noexcept;
            [[nodiscard]] TriangularRegion& lastNotChosenTriangularRegion() noexcept;
            [[nodiscard]] TriangularRegion& randomMinEntropyTriangularRegion(const uint_fast8_t index) noexcept;
            [[nodiscard]] const std::vector<Transversal::SymmetricCellUpdateData>& triangularDisable(
                const uint_fast16_t cellIndex, const uint_fast8_t regionIndex) noexcept;
            [[nodiscard]] const std::vector<Transversal::SymmetricCellUpdateData>& triangularDisableAndDecrease(
                const uint_fast16_t cellIndex, const uint_fast8_t regionIndex) noexcept;
            void triangularEnable(const uint_fast8_t regionIndex,
                const std::vector<Transversal::SymmetricCellUpdateData>& cellUpdateData) noexcept;
            void triangularEnableAndIncrease(
                const std::vector<Transversal::SymmetricCellUpdateData>& otherCellsUpdateData) noexcept;
            void triangularEnableAndIncrease(const Transversal::SymmetricCellUpdateData& updateData) noexcept;

        private:
            void reset() noexcept;
            void reset(const std::vector<uint_fast8_t>& numbers) noexcept;
            void resetRegions() noexcept;
            void resetNumberRegions() noexcept;

            uint_fast8_t size_;
            uint_fast16_t gridSize_;
            uint_fast16_t triangularGridSize_;
            uint_fast16_t entropyTriangularGridSize_;
            uint_fast8_t doubleSize_;
            uint_fast8_t maxUpdateSize_;
            uint_fast8_t regionsSize_;
            uint_fast8_t maxFillDiagonalSize_;
            uint_fast8_t maxDisableAndDecreaseSize_;
            uint_fast8_t maxOtherCellsUpdateData_;
            std::vector<std::shared_ptr<Cell>> grid_;
            std::vector<std::shared_ptr<Cell>> triangularGrid_;
            std::vector<std::shared_ptr<Cell>> entropyTriangularGrid_;
            std::vector<std::shared_ptr<Cell>> diagonalGrid_;
            std::vector<Region> regions_;
            std::vector<TriangularRegion> triangularRegions_;
            std::vector<TriangularRegion> numberRegions_;
            cpp::splitmix64 splitmix64_;
            Cell* minCell_;
            Region* minRegion_;
            TriangularRegion* minTriangularRegion_;
            uint_fast8_t minEntropy_;
            uint_fast16_t notFilled_;
            std::vector<uint_fast16_t> updateIndexes_;
            std::vector<uint_fast16_t> fillDiagonalIndexes_;
            std::vector<std::vector<std::shared_ptr<Cell>>> numberCells_;
            std::vector<uint_fast16_t> disableAndDecreaseIndexes_;
            std::vector<Transversal::SymmetricCellUpdateData> cellUpdateData_;
            std::vector<Transversal::SymmetricCellUpdateData> otherCellsUpdateData_;
    };
}
