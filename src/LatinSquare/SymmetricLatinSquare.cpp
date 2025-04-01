#include "SymmetricLatinSquare.hpp"



#include <iostream>

namespace LatinSquare {
    SymmetricLatinSquare::SymmetricLatinSquare(const uint_fast8_t size, const Type type) noexcept
        : size_(size) {
        set(type);
    }

    SymmetricLatinSquare::SymmetricLatinSquare(
        const uint_fast8_t size, const Type type, cpp::splitmix64& splitmix64) noexcept
        : size_(size), splitmix64_(splitmix64) {
        set(type);
    }

    SymmetricLatinSquare::SymmetricLatinSquare(
        const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers) noexcept
        : size_(size) {
        set(numbers);
    }

    SymmetricLatinSquare::SymmetricLatinSquare(
        const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers, cpp::splitmix64& splitmix64) noexcept
        : size_(size), splitmix64_(splitmix64) {
        set(numbers);
    }

    void SymmetricLatinSquare::set(const Type type) noexcept {
        if (grid_.size()) {
            reset();
            return;
        }

        gridSize_ = size_;
        gridSize_ *= size_;
        triangularGridSize_ = gridSize_;
        triangularGridSize_ += size_;
        triangularGridSize_ >>= 1;
        entropyTriangularGridSize_ = triangularGridSize_;
        entropyTriangularGridSize_ -= size_;
        doubleSize_ = size_;
        doubleSize_ <<= 1;
        maxUpdateSize_ = doubleSize_;
        maxUpdateSize_ -= 2;
        maxFillDiagonalSize_ = size_;
        notFilled_ = entropyTriangularGridSize_;
        grid_.resize(gridSize_);
        triangularGrid_.resize(triangularGridSize_);
        diagonalGrid_.resize(size_);
        triangularRegions_.reserve(size_);
        updateIndexes_.reserve(maxUpdateSize_);
        std::vector<std::vector<std::shared_ptr<Cell>>> cells;
        cells.resize(size_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            cells[index].reserve(size_);
        }

        uint_fast16_t triangularIndex = -1;
        uint_fast16_t diagonalIndex = -1;
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index] = std::make_shared<Cell>(index, row, column, size_, type);

            if (row >= column) {
                triangularGrid_[++triangularIndex] = grid_[index];

                if (row == column) {
                    diagonalGrid_[++diagonalIndex] = grid_[index];
                }

                cells[grid_[index]->rawRow()].emplace_back(grid_[index]);

                if (grid_[index]->notOnDiagonal()) {
                    cells[grid_[index]->rawColumn()].emplace_back(grid_[index]);
                }

                notFilled_ -= (row > column && grid_[index]->filled());
                maxFillDiagonalSize_ -= (row == column && grid_[index]->filled());
            }

            if (++column == size_) {
                column = 0;
                ++row;
            }
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            triangularRegions_.emplace_back(index, cells[index], size_);
        }

        entropyTriangularGrid_.resize(notFilled_);
        fillDiagonalIndexes_.reserve(maxFillDiagonalSize_);
        uint_fast16_t entropyTriangularIndex = -1;

        for (uint_fast16_t index = 0; index < triangularGridSize_; ++index) {
            if (triangularGrid_[index]->rawRow() > triangularGrid_[index]->rawColumn()
                && triangularGrid_[index]->notFilled()) {
                entropyTriangularGrid_[++entropyTriangularIndex] = triangularGrid_[index];
            }
        }
    }

    void SymmetricLatinSquare::reset() noexcept {
        notFilled_ = entropyTriangularGridSize_;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index]->reset();

            if (grid_[index]->rawRow() > grid_[index]->rawColumn()) {
                notFilled_ -= grid_[index]->filled();
            }
        }
    }

    void SymmetricLatinSquare::set(const std::vector<uint_fast8_t>& numbers) noexcept {
        if (grid_.size()) {
            reset(numbers);
            return;
        }

        gridSize_ = size_;
        gridSize_ *= size_;
        triangularGridSize_ = gridSize_;
        triangularGridSize_ += size_;
        triangularGridSize_ >>= 1;
        doubleSize_ = size_;
        doubleSize_ <<= 1;
        maxUpdateSize_ = doubleSize_;
        maxUpdateSize_ -= 2;
        maxFillDiagonalSize_ = size_;
        grid_.resize(gridSize_);
        triangularGrid_.resize(triangularGridSize_);
        diagonalGrid_.resize(size_);
        triangularRegions_.reserve(size_);
        updateIndexes_.reserve(maxUpdateSize_);
        std::vector<std::vector<std::shared_ptr<Cell>>> cells;
        cells.resize(size_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            cells[index].reserve(size_);
        }

        entropyTriangularGridSize_ = 0;
        uint_fast16_t triangularIndex = -1;
        uint_fast16_t diagonalIndex = -1;
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index] = std::make_shared<Cell>(index, row, column, size_, Type::Custom);

            if (row >= column) {
                triangularGrid_[++triangularIndex] = grid_[index];

                if (row == column) {
                    diagonalGrid_[++diagonalIndex] = grid_[index];
                }

                cells[grid_[index]->rawRow()].emplace_back(grid_[index]);

                if (grid_[index]->notOnDiagonal()) {
                    cells[grid_[index]->rawColumn()].emplace_back(grid_[index]);
                }

                maxFillDiagonalSize_ -= (row == column && grid_[index]->filled());
            }

            if (row > column) {
                entropyTriangularGridSize_ += (numbers[triangularIndex] == 0xFF);
            }

            if (++column == size_) {
                column = 0;
                ++row;
            }
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            triangularRegions_.emplace_back(index, cells[index], size_);
        }

        notFilled_ = entropyTriangularGridSize_;
        entropyTriangularGrid_.resize(entropyTriangularGridSize_);
        fillDiagonalIndexes_.reserve(maxFillDiagonalSize_);
        uint_fast16_t entropyTriangularIndex = -1;

        for (uint_fast16_t index = 0; index < triangularGridSize_; ++index) {
            if (numbers[index] == 0xFF) {
                if (triangularGrid_[index]->rawRow() > triangularGrid_[index]->rawColumn()) {
                    entropyTriangularGrid_[++entropyTriangularIndex] = triangularGrid_[index];
                }
            } else {
                triangularGrid_[index]->fillAndClear(numbers[index]);
                update(*triangularGrid_[index], numbers[index]);
            }
        }
    }

    void SymmetricLatinSquare::reset(const std::vector<uint_fast8_t>& numbers) noexcept {
        notFilled_ = entropyTriangularGridSize_;

        for (uint_fast16_t index = 0; index < triangularGridSize_; ++index) {
            triangularGrid_[index]->reset();

            if (numbers[index] != 0xFF) {
                triangularGrid_[index]->fillAndClear(numbers[index]);
                update(*triangularGrid_[index], numbers[index]);
            }
        }
    }

    void SymmetricLatinSquare::setNumberRegions() noexcept {
        if (numberRegions_.size()) {
            resetNumberRegions();
            return;
        }

        maxOtherCellsUpdateData_ = size_;
        --maxOtherCellsUpdateData_;
        maxOtherCellsUpdateData_ *= 4;
        numberRegions_.reserve(size_);
        numberCells_.resize(size_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            numberCells_[index].reserve(size_);
        }

        cellUpdateData_.reserve(2);
        otherCellsUpdateData_.reserve(maxOtherCellsUpdateData_);

        for (auto& cell : triangularGrid_) {
            numberCells_[cell->number()].emplace_back(cell);
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            numberRegions_.emplace_back(index, numberCells_[index], size_);
        }
    }

    void SymmetricLatinSquare::resetNumberRegions() noexcept {
        numberRegions_.clear();

        for (uint_fast8_t index = 0; index < size_; ++index) {
            numberCells_[index].clear();
        }

        for (auto& cell : triangularGrid_) {
            numberCells_[cell->number()].emplace_back(cell);
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            numberRegions_.emplace_back(index, numberCells_[index], size_);
        }
    }

    Cell& SymmetricLatinSquare::minEntropyCell() noexcept {
        Cell* minCell = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (auto& cell : entropyTriangularGrid_) {
            if (cell->filled()) {
                continue;
            }

            if (!cell->entropy()) {
                return *cell;
            }

            if (cell->entropy() < minEntropy) {
                minEntropy = cell->entropy();
                minCell = &(*cell);
            }
        }

        return *minCell;
    }

    Cell& SymmetricLatinSquare::randomMinEntropyCell() noexcept {
        Cell* minCell = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (auto& cell : entropyTriangularGrid_) {
            if (cell->filled()) {
                continue;
            }

            if (!cell->entropy()) {
                return *cell;
            }

            if (cell->entropy() < minEntropy) {
                minEntropy = cell->entropy();
                minCell = &(*cell);
            } else if (cell->entropy() == minEntropy && (splitmix64_.next() & 1)) { // TODO: maybe use better randomness
                minCell = &(*cell);
            }
        }

        return *minCell;
    }

    const std::vector<uint_fast16_t>& SymmetricLatinSquare::update(Cell& cell, const uint_fast8_t number) noexcept {
        updateIndexes_.clear();
        updateIndexes_ = triangularRegions_[cell.rawRow()].updatedCellIndexes(number);
        const auto& columnCellIndexes = triangularRegions_[cell.rawColumn()].updatedCellIndexes(number);
        updateIndexes_.insert(updateIndexes_.end(), columnCellIndexes.begin(), columnCellIndexes.end());
        return updateIndexes_;
    }

    uint_fast8_t SymmetricLatinSquare::checkDiagonal() noexcept {
        for (const auto& cell : diagonalGrid_) {
            if (cell->notFilled() && !cell->entropy()) {
                return 0;
            }
        }

        return 1;
    }

    void SymmetricLatinSquare::fillGrid() noexcept {
        if (grid_[0]->notFilled()) {
            grid_[0]->fill(triangularGrid_[0]->numbers()[0]);
            fillDiagonalIndexes_.emplace_back(0);
        }

        uint_fast16_t index = 0;
        uint_fast16_t triangularIndex = 1;

        for (uint_fast8_t columnSize = 1, columnIndex = 1; columnSize < size_; ++columnSize, ++columnIndex) {
            index = columnIndex;

            for (uint_fast8_t counter = 0; counter < columnSize; ++counter, index += size_, ++triangularIndex) {
                grid_[index]->fill(triangularGrid_[triangularIndex]->number());
            }

            if (grid_[index]->notFilled()) {
                grid_[index]->fill(triangularGrid_[triangularIndex]->numbers()[0]);
                fillDiagonalIndexes_.emplace_back(index);
            }

            ++triangularIndex;
        }
    }

    void SymmetricLatinSquare::fillDiagonal() noexcept {
        for (const auto& cell : diagonalGrid_) {
            if (cell->notFilled()) {
                cell->fill(cell->numbers()[0]);
                fillDiagonalIndexes_.emplace_back(cell->index());
            }
        }
    }

    void SymmetricLatinSquare::clearDiagonal() noexcept {
        for (const auto index : fillDiagonalIndexes_) {
            grid_[index]->clear();
        }

        fillDiagonalIndexes_.clear();
    }

    TriangularRegion& SymmetricLatinSquare::minEntropyTriangularRegion() noexcept {
        TriangularRegion* iterator = nullptr;
        uint_fast8_t minEntropy = 0xFF;
        uint_fast8_t triangularEnabled = 0xFF;

        for (auto& region : triangularRegions_) {
            if (region.notEnabled()) {
                continue;
            }

            if (!region.entropy()) {
                return region;
            }

            if (region.entropy() < minEntropy) {
                minEntropy = region.entropy();
                triangularEnabled = region.triangularEnabled();
                iterator = &region;
            } else if (region.entropy() == minEntropy && region.triangularEnabled() < triangularEnabled) {
                triangularEnabled = region.triangularEnabled();
                iterator = &region;
            }
        }

        return *iterator;
    }

    TriangularRegion& SymmetricLatinSquare::randomMinEntropyTriangularRegion() noexcept {
        TriangularRegion* iterator = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (auto& region : triangularRegions_) {
            if (region.notEnabled()) {
                continue;
            }

            if (!region.entropy()) {
                return region;
            }

            if (region.entropy() < minEntropy) {
                minEntropy = region.entropy();
                iterator = &region;
            } else if (region.entropy() == minEntropy && (splitmix64_.next() & 1)) { // TODO: maybe use better randomness
                iterator = &region;
            }
        }

        return *iterator;
    }

    const std::vector<Transversal::SymmetricCellUpdateData>& SymmetricLatinSquare::disable(
        const uint_fast16_t cellIndex, const uint_fast8_t regionIndex) noexcept {
        cellUpdateData_.clear();
        otherCellsUpdateData_.clear();
        const auto otherTriangularEnabled = grid_[cellIndex]->otherTriangularEnabled(regionIndex);
        grid_[cellIndex]->triangularDisable();
        triangularRegions_[regionIndex].disableAndDecrease();
        cellUpdateData_.emplace_back(cellIndex, regionIndex);

        if (!triangularRegions_[regionIndex].triangularEnabled()) {
            const auto& regionIndexes = triangularRegions_[regionIndex].triangularGlobalEnabledCellIndexes();

            for (const auto index : regionIndexes) {
                if (grid_[index]->triangularEnabled(regionIndex)) {
                    triangularRegions_[regionIndex].decrease();
                    otherCellsUpdateData_.emplace_back(index, regionIndex);
                }

                if (grid_[index]->notOnDiagonal() && grid_[index]->otherTriangularEnabled(regionIndex)) {
                    triangularRegions_[grid_[index]->otherRegionIndex(regionIndex)].decrease();
                    otherCellsUpdateData_.emplace_back(index, grid_[index]->otherRegionIndex(regionIndex));
                }

                grid_[index]->triangularDisable();
            }
        }

        if (grid_[cellIndex]->notOnDiagonal()
            && triangularRegions_[grid_[cellIndex]->otherRegionIndex(regionIndex)].triangularEnabled()) {
            if (otherTriangularEnabled) {
                triangularRegions_[grid_[cellIndex]->otherRegionIndex(regionIndex)].decrease();
            }

            triangularRegions_[grid_[cellIndex]->otherRegionIndex(regionIndex)].triangularDisable();
            cellUpdateData_.emplace_back(
                cellIndex, grid_[cellIndex]->otherRegionIndex(regionIndex), otherTriangularEnabled);

            if (!triangularRegions_[grid_[cellIndex]->otherRegionIndex(regionIndex)].triangularEnabled()) {
                const auto& regionIndexes = triangularRegions_[
                    grid_[cellIndex]->otherRegionIndex(regionIndex)].triangularGlobalEnabledCellIndexes();

                for (const auto index : regionIndexes) {
                    if (grid_[index]->triangularEnabled(grid_[cellIndex]->otherRegionIndex(regionIndex))) {
                        triangularRegions_[grid_[cellIndex]->otherRegionIndex(regionIndex)].decrease();
                        otherCellsUpdateData_.emplace_back(index, grid_[cellIndex]->otherRegionIndex(regionIndex));
                    }

                    if (grid_[index]->notOnDiagonal()
                        && grid_[index]->otherTriangularEnabled(grid_[cellIndex]->otherRegionIndex(regionIndex))) {
                        triangularRegions_[
                            grid_[index]->otherRegionIndex(grid_[cellIndex]->otherRegionIndex(regionIndex))].decrease();
                        otherCellsUpdateData_.emplace_back(
                            index, grid_[index]->otherRegionIndex(grid_[cellIndex]->otherRegionIndex(regionIndex)));
                    }

                    grid_[index]->triangularDisable();
                }
            }
        }

        return cellUpdateData_;
    }

    const std::vector<Transversal::SymmetricCellUpdateData>& SymmetricLatinSquare::disableAndDecrease(
        const uint_fast16_t cellIndex, const uint_fast8_t regionIndex) noexcept {
        if (grid_[cellIndex]->notOnDiagonal()) {
            if (diagonalGrid_[grid_[cellIndex]->rawColumn()]->triangularEnabled()) {
                diagonalGrid_[grid_[cellIndex]->rawColumn()]->triangularDisable();
                triangularRegions_[grid_[cellIndex]->rawColumn()].decrease();
                otherCellsUpdateData_.emplace_back(
                    diagonalGrid_[grid_[cellIndex]->rawColumn()]->index(), grid_[cellIndex]->rawColumn());
            }

            if (diagonalGrid_[grid_[cellIndex]->rawRow()]->triangularEnabled()) {
                diagonalGrid_[grid_[cellIndex]->rawRow()]->triangularDisable();
                triangularRegions_[grid_[cellIndex]->rawRow()].decrease();
                otherCellsUpdateData_.emplace_back(
                    diagonalGrid_[grid_[cellIndex]->rawRow()]->index(), grid_[cellIndex]->rawRow());
            }

            const auto& regionIndexes = triangularRegions_[regionIndex].triangularLocalEnabledCellIndexes();

            for (const auto index : regionIndexes) {
                grid_[index]->triangularDisable(regionIndex);
                triangularRegions_[regionIndex].decrease();
                otherCellsUpdateData_.emplace_back(index, regionIndex);
            }
        } else {
            const auto& regionIndexes = triangularRegions_[regionIndex].triangularGlobalEnabledCellIndexes();

            for (const auto index : regionIndexes) {
                if (grid_[index]->triangularEnabled(regionIndex)) {
                    triangularRegions_[regionIndex].decrease();
                    otherCellsUpdateData_.emplace_back(index, regionIndex);
                }

                if (grid_[index]->notOnDiagonal() && grid_[index]->otherTriangularEnabled(regionIndex)) {
                    triangularRegions_[grid_[index]->otherRegionIndex(regionIndex)].decrease();
                    otherCellsUpdateData_.emplace_back(index, grid_[index]->otherRegionIndex(regionIndex));
                }

                grid_[index]->triangularDisable();
            }
        }

        const auto& numberIndexes = numberRegions_[grid_[cellIndex]->number()].triangularGlobalEnabledCellIndexes();

        for (const auto index : numberIndexes) {
            if (grid_[index]->columnTriangularEnabled()) {
                triangularRegions_[grid_[index]->rawColumn()].decrease();
                otherCellsUpdateData_.emplace_back(index, grid_[index]->rawColumn());
            }

            if (grid_[index]->notOnDiagonal() && grid_[index]->rowTriangularEnabled()) {
                triangularRegions_[grid_[index]->rawRow()].decrease();
                otherCellsUpdateData_.emplace_back(index, grid_[index]->rawRow());
            }

            grid_[index]->triangularDisable();
        }

        return otherCellsUpdateData_;
    }

    void SymmetricLatinSquare::enable(const uint_fast8_t regionIndex,
        const std::vector<Transversal::SymmetricCellUpdateData>& cellUpdateData) noexcept {
        triangularRegions_[regionIndex].enable();

        for (const auto& updateData : cellUpdateData) {
            if (updateData.regionIndex() != regionIndex) {
                triangularRegions_[updateData.regionIndex()].triangularEnable();

                if (updateData.enabled()) {
                    grid_[updateData.cellIndex()]->triangularEnable(updateData.regionIndex());
                    triangularRegions_[updateData.regionIndex()].increase();
                }
            }
        }
    }

    void SymmetricLatinSquare::enableAndIncrease(
        const std::vector<Transversal::SymmetricCellUpdateData>& otherCellsUpdateData) noexcept {
        for (const auto& updateData : otherCellsUpdateData) {
            grid_[updateData.cellIndex()]->triangularEnable(updateData.regionIndex());
            triangularRegions_[updateData.regionIndex()].increase();
        }
    }

    void SymmetricLatinSquare::enableAndIncrease(const uint_fast8_t regionIndex,
        const std::vector<Transversal::SymmetricCellUpdateData>& cellsUpdateData) noexcept {
        for (const auto& updateData : cellsUpdateData) {
            if (updateData.regionIndex() == regionIndex) {
                grid_[updateData.cellIndex()]->triangularEnable(regionIndex);
                triangularRegions_[regionIndex].increase();
            }
        }
    }
}
