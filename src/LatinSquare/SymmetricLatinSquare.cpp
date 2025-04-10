#include "SymmetricLatinSquare.hpp"

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
        maxUpdateSize_ = size_;
        --maxUpdateSize_;
        maxUpdateSize_ *= 2;
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
        maxUpdateSize_ = size_;
        --maxUpdateSize_;
        maxUpdateSize_ *= 2;
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
                entropyTriangularGridSize_ += (numbers[triangularIndex] == DEFAULT_NUMBER);
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
            if (numbers[index] == DEFAULT_NUMBER) {
                if (triangularGrid_[index]->rawRow() > triangularGrid_[index]->rawColumn()) {
                    entropyTriangularGrid_[++entropyTriangularIndex] = triangularGrid_[index];
                }

                continue;
            }

            triangularGrid_[index]->fillAndClear(numbers[index]);
            update(*triangularGrid_[index], numbers[index]);
        }
    }

    void SymmetricLatinSquare::reset(const std::vector<uint_fast8_t>& numbers) noexcept {
        notFilled_ = entropyTriangularGridSize_;

        for (uint_fast16_t index = 0; index < triangularGridSize_; ++index) {
            triangularGrid_[index]->reset();

            if (numbers[index] != DEFAULT_NUMBER) {
                triangularGrid_[index]->fillAndClear(numbers[index]);
                update(*triangularGrid_[index], numbers[index]);
            }
        }
    }

    void SymmetricLatinSquare::setRegions() noexcept {
        if (regions_.size()) {
            resetRegions();
            return;
        }

        regionsSize_ = size_;
        regionsSize_ *= 3;
        maxDisableAndDecreaseSize_ = size_;
        --maxDisableAndDecreaseSize_;
        maxDisableAndDecreaseSize_ *= 3;
        regions_.reserve(regionsSize_);
        std::vector<std::vector<std::shared_ptr<Cell>>> rowCells, columnCells;
        rowCells.resize(size_);
        columnCells.resize(size_);
        numberCells_.resize(size_);
        disableAndDecreaseIndexes_.reserve(maxDisableAndDecreaseSize_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            rowCells[index].reserve(size_);
            columnCells[index].reserve(size_);
            numberCells_[index].reserve(size_);
        }

        for (auto& cell : grid_) {
            cell->setRegionNumber();
            rowCells[cell->rawRow()].emplace_back(cell);
            columnCells[cell->rawColumn()].emplace_back(cell);
            numberCells_[cell->number()].emplace_back(cell);
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            regions_.emplace_back(index, rowCells[index], size_);
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            regions_.emplace_back(index + size_, columnCells[index], size_);
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            regions_.emplace_back(index + doubleSize_, numberCells_[index], size_);
        }
    }

    void SymmetricLatinSquare::resetRegions() noexcept {
        regions_.resize(doubleSize_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            numberCells_[index].clear();
        }

        for (auto& cell : grid_) {
            cell->setRegionNumber();
            numberCells_[cell->number()].emplace_back(cell);
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            regions_.emplace_back(index + doubleSize_, numberCells_[index], size_);
        }
    }

    void SymmetricLatinSquare::setNumberRegions() noexcept {
        if (numberRegions_.size()) {
            resetNumberRegions();
            return;
        }

        maxOtherCellsUpdateData_ = size_;
        --maxOtherCellsUpdateData_;
        maxOtherCellsUpdateData_ *= 3;
        numberRegions_.reserve(size_);
        numberCells_.resize(size_);
        cellUpdateData_.reserve(2);
        otherCellsUpdateData_.reserve(maxOtherCellsUpdateData_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            numberCells_[index].clear();
        }

        for (auto& cell : triangularGrid_) {
            numberCells_[cell->number()].emplace_back(cell);
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            numberRegions_.emplace_back(size_, numberCells_[index], size_);
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
            numberRegions_.emplace_back(size_, numberCells_[index], size_);
        }
    }

    Cell& SymmetricLatinSquare::minEntropyCell(const uint_fast16_t index) noexcept {
        if (index < DEFAULT_CELL_INDEX) {
            return *grid_[index];
        }

        minCell_ = nullptr;
        minEntropy_ = DEFAULT_ENTROPY;

        for (auto& cell : entropyTriangularGrid_) {
            if (cell->filled()) {
                continue;
            }

            if (!cell->positiveEntropy()) {
                return *cell;
            }

            if (cell->entropy() < minEntropy_) {
                minCell_ = &(*cell);
                minEntropy_ = cell->entropy();
            }
        }

        return *minCell_;
    }

    Cell& SymmetricLatinSquare::randomMinEntropyCell(const uint_fast16_t index) noexcept {
        if (index < DEFAULT_CELL_INDEX) {
            return *grid_[index];
        }

        minCell_ = nullptr;
        minEntropy_ = DEFAULT_ENTROPY;

        for (auto& cell : entropyTriangularGrid_) {
            if (cell->filled()) {
                continue;
            }

            if (!cell->positiveEntropy()) {
                return *cell;
            }

            if (cell->entropy() < minEntropy_) {
                minCell_ = &(*cell);
                minEntropy_ = cell->entropy();
                continue;
            }

            if (cell->entropy() == minEntropy_ && splitmix64_.next() % notFilled_ == 0) {
                minCell_ = &(*cell);
            }
        }

        return *minCell_;
    }

    const std::vector<uint_fast16_t>& SymmetricLatinSquare::update(Cell& cell, const uint_fast8_t number) noexcept {
        updateIndexes_.clear();
        updateIndexes_ = triangularRegions_[cell.rawRow()].updatedCellIndexes(number);
        const auto& columnCellIndexes = triangularRegions_[cell.rawColumn()].updatedCellIndexes(number);
        updateIndexes_.insert(updateIndexes_.end(), columnCellIndexes.begin(), columnCellIndexes.end());

        if (cell.type() == Type::ReducedDiagonal && !cell.notOnDiagonal()) {
            for (uint_fast16_t index = 0; index < gridSize_; index += size_) {
                if (grid_[index]->notFilled() && grid_[index]->canBeRemoved(number)) {
                    grid_[index]->remove();
                    updateIndexes_.emplace_back(index);
                }

                ++index;
            }
        }

        return updateIndexes_;
    }

    uint_fast8_t SymmetricLatinSquare::checkDiagonal() noexcept {
        for (const auto& cell : diagonalGrid_) {
            if (cell->notFilled() && !cell->positiveEntropy()) {
                return 0;
            }
        }

        return 1;
    }

    void SymmetricLatinSquare::fillGrid() noexcept {
        if (grid_[0]->notFilled()) {
            grid_[0]->fill(triangularGrid_[0]->firstNumber());
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
                grid_[index]->fill(triangularGrid_[triangularIndex]->firstNumber());
                fillDiagonalIndexes_.emplace_back(index);
            }

            ++triangularIndex;
        }
    }

    void SymmetricLatinSquare::fillDiagonal() noexcept {
        for (const auto& cell : diagonalGrid_) {
            if (cell->notFilled()) {
                cell->fill(cell->firstNumber());
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

    Region& SymmetricLatinSquare::minEntropyRegion(const uint_fast8_t index) noexcept {
        if (index < DEFAULT_REGION_INDEX) {
            return regions_[index];
        }

        minRegion_ = nullptr;
        minEntropy_ = DEFAULT_ENTROPY;

        for (auto& region : regions_) {
            if (region.notEnabled()) {
                continue;
            }

            if (!region.entropy()) {
                return region;
            }

            if (region.entropy() < minEntropy_) {
                minRegion_ = &region;
                minEntropy_ = region.entropy();
            }
        }

        return *minRegion_;
    }

    Region& SymmetricLatinSquare::lastNotChosenRegion() noexcept {
        for (auto& region : regions_) {
            if (!region.notEnabled()) {
                return region;
            }
        }

        return regions_[0];
    }

    Region& SymmetricLatinSquare::randomMinEntropyRegion(const uint_fast8_t index) noexcept {
        if (index < DEFAULT_REGION_INDEX) {
            return regions_[index];
        }

        minRegion_ = nullptr;
        minEntropy_ = DEFAULT_ENTROPY;

        for (auto& region : regions_) {
            if (region.notEnabled()) {
                continue;
            }

            if (!region.entropy()) {
                return region;
            }

            if (region.entropy() < minEntropy_) {
                minRegion_ = &region;
                minEntropy_ = region.entropy();
                continue;
            }

            if (region.entropy() == minEntropy_ && splitmix64_.next() % regionsSize_ == 0) {
                minRegion_ = &region;
            }
        }

        return *minRegion_;
    }

    void SymmetricLatinSquare::disable(const uint_fast16_t index) noexcept {
        grid_[index]->disable();
        regions_[grid_[index]->regionRow()].disableAndDecrease();
        regions_[grid_[index]->regionColumn()].disableAndDecrease();
        regions_[grid_[index]->regionNumber()].disableAndDecrease();
    }

    const std::vector<uint_fast16_t>& SymmetricLatinSquare::disableAndDecrease(const uint_fast16_t index) noexcept {
        disableAndDecreaseIndexes_.clear();
        uint_fast16_t rowIndex = grid_[index]->rawRow();
        rowIndex *= size_;
        uint_fast16_t columnIndex = grid_[index]->rawColumn();

        while (columnIndex < gridSize_) {
            if (grid_[rowIndex]->enabled()) {
                grid_[rowIndex]->disable();
                regions_[grid_[rowIndex]->regionRow()].decrease();
                regions_[grid_[rowIndex]->regionColumn()].decrease();
                regions_[grid_[rowIndex]->regionNumber()].decrease();
                disableAndDecreaseIndexes_.emplace_back(rowIndex);
            }

            if (grid_[columnIndex]->enabled()) {
                grid_[columnIndex]->disable();
                regions_[grid_[columnIndex]->regionRow()].decrease();
                regions_[grid_[columnIndex]->regionColumn()].decrease();
                regions_[grid_[columnIndex]->regionNumber()].decrease();
                disableAndDecreaseIndexes_.emplace_back(columnIndex);
            }

            ++rowIndex;
            columnIndex += size_;
        }

        const auto& numberIndexes = regions_[grid_[index]->regionNumber()].enabledCellIndexes();

        for (const auto numberIndex : numberIndexes) {
            grid_[numberIndex]->disable();
            regions_[grid_[numberIndex]->regionRow()].decrease();
            regions_[grid_[numberIndex]->regionColumn()].decrease();
            regions_[grid_[numberIndex]->regionNumber()].decrease();
            disableAndDecreaseIndexes_.emplace_back(numberIndex);
        }

        return disableAndDecreaseIndexes_;
    }

    void SymmetricLatinSquare::enable(const uint_fast16_t index) noexcept {
        regions_[grid_[index]->regionRow()].enable();
        regions_[grid_[index]->regionColumn()].enable();
        regions_[grid_[index]->regionNumber()].enable();
    }

    void SymmetricLatinSquare::enableAndIncrease(const uint_fast16_t index) noexcept {
        grid_[index]->enable();
        regions_[grid_[index]->regionRow()].increase();
        regions_[grid_[index]->regionColumn()].increase();
        regions_[grid_[index]->regionNumber()].increase();
    }

    void SymmetricLatinSquare::enableAndIncrease(const std::vector<uint_fast16_t>& indexes) noexcept {
        for (const auto index : indexes) {
            grid_[index]->enable();
            regions_[grid_[index]->regionRow()].increase();
            regions_[grid_[index]->regionColumn()].increase();
            regions_[grid_[index]->regionNumber()].increase();
        }
    }

    TriangularRegion& SymmetricLatinSquare::minEntropyTriangularRegion(const uint_fast8_t index) noexcept {
        if (index < DEFAULT_REGION_INDEX) {
            return triangularRegions_[index];
        }

        minTriangularRegion_ = nullptr;
        minEntropy_ = DEFAULT_ENTROPY;

        for (auto& region : triangularRegions_) {
            if (region.notEnabled()) {
                continue;
            }

            if (!region.entropy()) {
                return region;
            }

            if (region.entropy() < minEntropy_) {
                minTriangularRegion_ = &region;
                minEntropy_ = region.entropy();
            }
        }

        return *minTriangularRegion_;
    }

    TriangularRegion& SymmetricLatinSquare::lastNotChosenTriangularRegion() noexcept {
        for (auto& region : triangularRegions_) {
            if (!region.notEnabled()) {
                return region;
            }
        }

        return triangularRegions_[0];
    }

    TriangularRegion& SymmetricLatinSquare::randomMinEntropyTriangularRegion(const uint_fast8_t index) noexcept {
        if (index < DEFAULT_REGION_INDEX) {
            return triangularRegions_[index];
        }

        minTriangularRegion_ = nullptr;
        minEntropy_ = DEFAULT_ENTROPY;

        for (auto& region : triangularRegions_) {
            if (region.notEnabled()) {
                continue;
            }

            if (!region.entropy()) {
                return region;
            }

            if (region.entropy() < minEntropy_) {
                minTriangularRegion_ = &region;
                minEntropy_ = region.entropy();
                continue;
            }

            if (region.entropy() == minEntropy_ && splitmix64_.next() % size_ == 0) {
                minTriangularRegion_ = &region;
            }
        }

        return *minTriangularRegion_;
    }

    const std::vector<Transversal::SymmetricCellUpdateData>& SymmetricLatinSquare::triangularDisable(
        const uint_fast16_t cellIndex, const uint_fast8_t regionIndex) noexcept {
        cellUpdateData_.clear();
        grid_[cellIndex]->triangularDisable(regionIndex);
        triangularRegions_[regionIndex].disableAndDecrease();
        cellUpdateData_.emplace_back(cellIndex, regionIndex);

        if (grid_[cellIndex]->triangularEnabled()) {
            const auto cellOtherRegionIndex = grid_[cellIndex]->otherRegionIndex(regionIndex);
            grid_[cellIndex]->triangularDisable(cellOtherRegionIndex);
            triangularRegions_[cellOtherRegionIndex].decrease();
            cellUpdateData_.emplace_back(cellIndex, cellOtherRegionIndex);
        }

        return cellUpdateData_;
    }

    const std::vector<Transversal::SymmetricCellUpdateData>& SymmetricLatinSquare::triangularDisableAndDecrease(
        const uint_fast16_t cellIndex, const uint_fast8_t regionIndex) noexcept {
        otherCellsUpdateData_.clear();
        const auto cellOtherRegionIndex = grid_[cellIndex]->otherRegionIndex(regionIndex);
        const auto& otherRegionIndexes =
            triangularRegions_[cellOtherRegionIndex].triangularOtherLocalEnabledCellIndexes();

        for (const auto index : otherRegionIndexes) {
            const auto otherRegionIndex = grid_[index]->otherRegionIndex(cellOtherRegionIndex);
            grid_[index]->triangularDisable(otherRegionIndex);
            triangularRegions_[otherRegionIndex].decrease();
            otherCellsUpdateData_.emplace_back(index, otherRegionIndex);
        }

        const auto& numberIndexes = numberRegions_[grid_[cellIndex]->number()].triangularGlobalEnabledCellIndexes();

        for (const auto index : numberIndexes) {
            if (grid_[index]->columnTriangularEnabled()) {
                grid_[index]->triangularDisable(grid_[index]->rawColumn());
                triangularRegions_[grid_[index]->rawColumn()].decrease();
                otherCellsUpdateData_.emplace_back(index, grid_[index]->rawColumn());
            }

            if (grid_[index]->rowTriangularEnabled()) {
                grid_[index]->triangularDisable(grid_[index]->rawRow());
                triangularRegions_[grid_[index]->rawRow()].decrease();
                otherCellsUpdateData_.emplace_back(index, grid_[index]->rawRow());
            }
        }

        return otherCellsUpdateData_;
    }

    void SymmetricLatinSquare::triangularEnable(const uint_fast8_t regionIndex,
        const std::vector<Transversal::SymmetricCellUpdateData>& cellUpdateData) noexcept {
        triangularRegions_[regionIndex].enable();

        if (cellUpdateData.size() > 1) {
            grid_[cellUpdateData[1].cellIndex()]->triangularEnable(cellUpdateData[1].regionIndex());
            triangularRegions_[cellUpdateData[1].regionIndex()].increase();
        }
    }

    void SymmetricLatinSquare::triangularEnableAndIncrease(
        const std::vector<Transversal::SymmetricCellUpdateData>& otherCellsUpdateData) noexcept {
        for (const auto& updateData : otherCellsUpdateData) {
            grid_[updateData.cellIndex()]->triangularEnable(updateData.regionIndex());
            triangularRegions_[updateData.regionIndex()].increase();
        }
    }

    void SymmetricLatinSquare::triangularEnableAndIncrease(
        const Transversal::SymmetricCellUpdateData& updateData) noexcept {
        grid_[updateData.cellIndex()]->triangularEnable(updateData.regionIndex());
        triangularRegions_[updateData.regionIndex()].increase();
    }
}
