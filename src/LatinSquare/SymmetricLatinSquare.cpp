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
        gridSize_ = size_;
        gridSize_ *= size_;

        if (numbers.size() < gridSize_) {
            triangularSet(numbers);
        } else {
            set(numbers);
        }
    }

    SymmetricLatinSquare::SymmetricLatinSquare(
        const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers, cpp::splitmix64& splitmix64) noexcept
        : size_(size), splitmix64_(splitmix64) {
        gridSize_ = size_;
        gridSize_ *= size_;

        if (numbers.size() < gridSize_) {
            triangularSet(numbers);
        } else {
            set(numbers);
        }
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
        notFilled_ = entropyTriangularGridSize_;
        grid_.resize(gridSize_);
        triangularGrid_.resize(triangularGridSize_);
        triangularRegions_.reserve(size_);
        updateIndexes_.reserve(maxUpdateSize_);
        fillGridIndexes_.reserve(size_);
        std::vector<std::vector<std::shared_ptr<Cell>>> cells;
        cells.resize(size_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            cells[index].reserve(size_);
        }

        uint_fast16_t triangularIndex = -1;
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index] = std::make_shared<Cell>(index, row, column, size_, type);

            if (row >= column) {
                triangularGrid_[++triangularIndex] = grid_[index];
                cells[grid_[index]->rawRow()].emplace_back(grid_[index]);
                cells[grid_[index]->rawColumn()].emplace_back(grid_[index]);
                notFilled_ -= (row > column && grid_[index]->filled());
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

    void SymmetricLatinSquare::triangularSet(const std::vector<uint_fast8_t>& numbers) noexcept {
        if (grid_.size()) {
            triangularReset(numbers);
            return;
        }

        triangularGridSize_ = gridSize_;
        triangularGridSize_ += size_;
        triangularGridSize_ >>= 1;
        doubleSize_ = size_;
        doubleSize_ <<= 1;
        maxUpdateSize_ = doubleSize_;
        maxUpdateSize_ -= 2;
        grid_.resize(gridSize_);
        triangularGrid_.resize(triangularGridSize_);
        triangularRegions_.reserve(size_);
        updateIndexes_.reserve(maxUpdateSize_);
        std::vector<std::vector<std::shared_ptr<Cell>>> cells;
        cells.resize(size_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            cells[index].reserve(size_);
        }

        entropyTriangularGridSize_ = 0;
        uint_fast16_t triangularIndex = -1;
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index] = std::make_shared<Cell>(index, row, column, size_, Type::Custom);

            if (row >= column) {
                triangularGrid_[++triangularIndex] = grid_[index];
                cells[grid_[index]->rawRow()].emplace_back(grid_[index]);
                cells[grid_[index]->rawColumn()].emplace_back(grid_[index]);
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

    void SymmetricLatinSquare::triangularReset(const std::vector<uint_fast8_t>& numbers) noexcept {
        notFilled_ = entropyTriangularGridSize_;

        for (uint_fast16_t index = 0; index < triangularGridSize_; ++index) {
            triangularGrid_[index]->reset();

            if (numbers[index] != 0xFF) {
                triangularGrid_[index]->fillAndClear(numbers[index]);
                update(*triangularGrid_[index], numbers[index]);
            }
        }
    }

    void SymmetricLatinSquare::set(const std::vector<uint_fast8_t>& numbers) noexcept {
        if (grid_.size()) {
            reset(numbers);
            return;
        }

        doubleSize_ = size_;
        doubleSize_ <<= 1;
        notFilled_ = gridSize_;
        grid_.resize(gridSize_);
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index] = std::make_shared<Cell>(index, row, column, size_, Type::Custom);

            if (numbers[index] != 0xFF) {
                grid_[index]->fillAndClear(numbers[index]);
                --notFilled_;
            }

            if (++column == size_) {
                column = 0;
                ++row;
            }
        }
    }

    void SymmetricLatinSquare::reset(const std::vector<uint_fast8_t>& numbers) noexcept {
        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index]->reset();
            grid_[index]->fillAndClear(numbers[index]);
        }
    }

    void SymmetricLatinSquare::setRegions() noexcept {
        if (regions_.size()) {
            resetRegions();
            return;
        }

        regionsSize_ = size_;
        regionsSize_ *= 3;
        maxDisableAndDecreaseSize_ = regionsSize_;
        maxDisableAndDecreaseSize_ -= 3;
        regions_.reserve(regionsSize_);
        std::vector<std::vector<std::shared_ptr<Cell>>> rowCells, columnCells;
        rowCells.resize(size_);
        columnCells.resize(size_);
        numberCells_.resize(size_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            rowCells[index].reserve(size_);
            columnCells[index].reserve(size_);
            numberCells_[index].reserve(size_);
        }

        disableAndDecreaseIndexes_.reserve(maxDisableAndDecreaseSize_);

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
            uint_fast8_t columnIndex = index;
            columnIndex += size_;
            regions_.emplace_back(columnIndex, columnCells[index], size_);
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            uint_fast8_t numberIndex = index;
            numberIndex += doubleSize_;
            regions_.emplace_back(numberIndex, numberCells_[index], size_);
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
            uint_fast8_t numberIndex = index;
            numberIndex += doubleSize_;
            regions_.emplace_back(numberIndex, numberCells_[index], size_);
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
        for (uint_fast16_t index = 0, step = 1; index < triangularGridSize_; index += ++step) {
            if (triangularGrid_[index]->notFilled() && !triangularGrid_[index]->entropy()) {
                return 0;
            }
        }

        return 1;
    }

    void SymmetricLatinSquare::fillGrid() noexcept {
        if (grid_[0]->notFilled()) {
            grid_[0]->fill(triangularGrid_[0]->numbers()[0]);
            fillGridIndexes_.emplace_back(0);
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
                fillGridIndexes_.emplace_back(index);
            }

            ++triangularIndex;
        }
    }

    void SymmetricLatinSquare::clearGrid() noexcept {
        for (const auto index : fillGridIndexes_) {
            grid_[index]->clear();
        }

        fillGridIndexes_.clear();
    }

    Region& SymmetricLatinSquare::minEntropyRegion() noexcept {
        Region* iterator = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (auto& region : regions_) {
            if (region.notEnabled()) {
                continue;
            }

            if (!region.entropy()) {
                return region;
            }

            if (region.entropy() < minEntropy) {
                minEntropy = region.entropy();
                iterator = &region;
            }
        }

        return *iterator;
    }

    Region& SymmetricLatinSquare::randomMinEntropyRegion() noexcept {
        Region* iterator = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (auto& region : regions_) {
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

    void SymmetricLatinSquare::enableAndIncrease(const std::vector<uint_fast16_t>& indexes) noexcept {
        for (const auto index : indexes) {
            grid_[index]->enable();
            regions_[grid_[index]->regionRow()].increase();
            regions_[grid_[index]->regionColumn()].increase();
            regions_[grid_[index]->regionNumber()].increase();
        }
    }
}
