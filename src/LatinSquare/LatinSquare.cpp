#include "LatinSquare.hpp"

namespace LatinSquare {
    LatinSquare::LatinSquare(const uint_fast8_t size, const Type type) noexcept
        : size_(size) {
        set(type);
    }

    LatinSquare::LatinSquare(const uint_fast8_t size, const Type type, cpp::splitmix64& splitmix64) noexcept
        : size_(size), splitmix64_(splitmix64) {
        set(type);
    }

    LatinSquare::LatinSquare(const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers) noexcept
        : size_(size) {
        set(numbers);
    }

    LatinSquare::LatinSquare(
        const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers, cpp::splitmix64& splitmix64) noexcept
        : size_(size), splitmix64_(splitmix64) {
        set(numbers);
    }

    void LatinSquare::set(const Type type) noexcept {
        if (grid_.size()) {
            reset();
            return;
        }

        gridSize_ = size_;
        gridSize_ *= size_;
        doubleSize_ = size_;
        doubleSize_ <<= 1;
        maxUpdateSize_ = size_;
        --maxUpdateSize_;
        maxUpdateSize_ *= 3;
        notFilled_ = gridSize_;
        grid_.resize(gridSize_);
        updateIndexes_.reserve(maxUpdateSize_);
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index] = std::make_shared<Cell>(index, row, column, size_, type);
            notFilled_ -= grid_[index]->filled();

            if (++column == size_) {
                column = 0;
                ++row;
            }
        }

        entropyGrid_.resize(notFilled_);
        uint_fast16_t entropyIndex = -1;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            if (grid_[index]->notFilled()) {
                entropyGrid_[++entropyIndex] = grid_[index];
            }
        }
    }

    void LatinSquare::reset() noexcept {
        notFilled_ = gridSize_;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index]->reset();
            notFilled_ -= grid_[index]->filled();
        }
    }

    void LatinSquare::set(const std::vector<uint_fast8_t>& numbers) noexcept {
        if (grid_.size()) {
            reset(numbers);
            return;
        }

        gridSize_ = size_;
        gridSize_ *= size_;
        doubleSize_ = size_;
        doubleSize_ <<= 1;
        maxUpdateSize_ = size_;
        --maxUpdateSize_;
        maxUpdateSize_ *= 3;
        notFilled_ = gridSize_;
        grid_.resize(gridSize_);
        updateIndexes_.reserve(maxUpdateSize_);
        entropyGridSize_ = 0;
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index] = std::make_shared<Cell>(index, row, column, size_, Type::Custom);
            entropyGridSize_ += (numbers[index] == DEFAULT_NUMBER);

            if (++column == size_) {
                column = 0;
                ++row;
            }
        }

        notFilled_ = entropyGridSize_;
        entropyGrid_.resize(entropyGridSize_);
        uint_fast16_t entropyIndex = -1;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            if (numbers[index] == DEFAULT_NUMBER) {
                entropyGrid_[++entropyIndex] = grid_[index];
            } else {
                grid_[index]->fillAndClear(numbers[index]);
                update(*grid_[index], numbers[index]);
            }
        }
    }

    void LatinSquare::reset(const std::vector<uint_fast8_t>& numbers) noexcept {
        notFilled_ = entropyGridSize_;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index]->reset();

            if (numbers[index] != DEFAULT_NUMBER) {
                grid_[index]->fillAndClear(numbers[index]);
                update(*grid_[index], numbers[index]);
            }
        }
    }

    void LatinSquare::setRegions() noexcept {
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
            regions_.emplace_back(index + size_, columnCells[index], size_);
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            regions_.emplace_back(index + doubleSize_, numberCells_[index], size_);
        }
    }

    void LatinSquare::resetRegions() noexcept {
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

    Cell& LatinSquare::minEntropyCell(const uint_fast16_t index) noexcept {
        if (index < DEFAULT_CELL_INDEX) {
            return *grid_[index];
        }

        minCell_ = nullptr;
        minEntropy_ = DEFAULT_ENTROPY;

        for (auto& cell : entropyGrid_) {
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

    Cell& LatinSquare::lastNotFilledCell() noexcept {
        for (auto& cell : entropyGrid_) {
            if (cell->notFilled()) {
                return *cell;
            }
        }

        return *entropyGrid_[0];
    }

    Cell& LatinSquare::randomMinEntropyCell(const uint_fast16_t index) noexcept {
        if (index < DEFAULT_CELL_INDEX) {
            return *grid_[index];
        }

        minCell_ = nullptr;
        minEntropy_ = DEFAULT_ENTROPY;

        for (auto& cell : entropyGrid_) {
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

    const std::vector<uint_fast16_t>& LatinSquare::update(Cell& cell, const uint_fast8_t number) noexcept {
        updateIndexes_.clear();
        uint_fast16_t rowIndex = cell.rawRow();
        rowIndex *= size_;
        uint_fast16_t columnIndex = cell.rawColumn();

        while (columnIndex < gridSize_) {
            if (grid_[rowIndex]->notFilled() && grid_[rowIndex]->canBeRemoved(number)) {
                grid_[rowIndex]->remove();
                updateIndexes_.emplace_back(rowIndex);
            }

            if (grid_[columnIndex]->notFilled() && grid_[columnIndex]->canBeRemoved(number)) {
                grid_[columnIndex]->remove();
                updateIndexes_.emplace_back(columnIndex);
            }

            ++rowIndex;
            columnIndex += size_;
        }

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

    Region& LatinSquare::minEntropyRegion(const uint_fast8_t index) noexcept {
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

    Region& LatinSquare::lastNotChosenRegion() noexcept {
        for (auto& region : regions_) {
            if (!region.notEnabled()) {
                return region;
            }
        }

        return regions_[0];
    }

    Region& LatinSquare::randomMinEntropyRegion(const uint_fast8_t index) noexcept {
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

    void LatinSquare::disable(const uint_fast16_t index) noexcept {
        grid_[index]->disable();
        regions_[grid_[index]->regionRow()].disableAndDecrease();
        regions_[grid_[index]->regionColumn()].disableAndDecrease();
        regions_[grid_[index]->regionNumber()].disableAndDecrease();
    }

    const std::vector<uint_fast16_t>& LatinSquare::disableAndDecrease(const uint_fast16_t index) noexcept {
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

    void LatinSquare::enable(const uint_fast16_t index) noexcept {
        regions_[grid_[index]->regionRow()].enable();
        regions_[grid_[index]->regionColumn()].enable();
        regions_[grid_[index]->regionNumber()].enable();
    }

    void LatinSquare::enableAndIncrease(const uint_fast16_t index) noexcept {
        grid_[index]->enable();
        regions_[grid_[index]->regionRow()].increase();
        regions_[grid_[index]->regionColumn()].increase();
        regions_[grid_[index]->regionNumber()].increase();
    }

    void LatinSquare::enableAndIncrease(const std::vector<uint_fast16_t>& indexes) noexcept {
        for (const auto index : indexes) {
            grid_[index]->enable();
            regions_[grid_[index]->regionRow()].increase();
            regions_[grid_[index]->regionColumn()].increase();
            regions_[grid_[index]->regionNumber()].increase();
        }
    }
}
