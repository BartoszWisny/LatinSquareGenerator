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

        grid_.clear();
        entropyGrid_.clear();
        gridSize_ = size_;
        gridSize_ *= size_;
        notFilled_ = gridSize_;
        grid_.resize(gridSize_);
        entropyGrid_.reserve(gridSize_);
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index] = std::make_shared<Cell>(index, row, column, size_, type);

            if (grid_[index]->filled()) {
                --notFilled_;
            } else {
                entropyGrid_.emplace_back(grid_[index]);
            }

            if (++column == size_) {
                column = 0;
                ++row;
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

        grid_.clear();
        entropyGrid_.clear();
        gridSize_ = size_;
        gridSize_ *= size_;
        notFilled_ = gridSize_;
        grid_.resize(gridSize_);
        entropyGridSize_ = 0;
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            grid_[index] = std::make_shared<Cell>(index, row, column, size_, Type::Custom);
            entropyGridSize_ += (numbers[index] == 0xFF);

            if (++column == size_) {
                column = 0;
                ++row;
            }
        }

        notFilled_ = entropyGridSize_;
        entropyGrid_.resize(entropyGridSize_);
        uint_fast16_t entropyIndex = -1;

        for (uint_fast16_t index = 0; index < gridSize_; ++index) {
            if (numbers[index] == 0xFF) {
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

            if (numbers[index] != 0xFF) {
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

        for (auto& cell : grid_) {
            cell->enable();
        }

        doubleSize_ = size_;
        doubleSize_ <<= 1;
        regionsSize_ = size_;
        regionsSize_ *= 3;
        regions_.reserve(regionsSize_);
        std::vector<std::vector<std::shared_ptr<Cell>>> rowCells, columnCells, numberCells;
        rowCells.resize(size_);
        columnCells.resize(size_);
        numberCells.resize(size_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            rowCells[index].reserve(size_);
            columnCells[index].reserve(size_);
            numberCells[index].reserve(size_);
        }

        for (auto& cell : grid_) {
            rowCells[cell->rawRow()].emplace_back(cell);
            columnCells[cell->rawColumn()].emplace_back(cell);
            numberCells[cell->number()].emplace_back(cell);
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
            regions_.emplace_back(numberIndex, numberCells[index], size_);
        }
    }

    void LatinSquare::resetRegions() noexcept {
        std::vector<std::vector<std::shared_ptr<Cell>>> numberCells;
        numberCells.resize(size_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            numberCells[index].reserve(size_);
        }

        for (auto& cell : grid_) {
            numberCells[cell->number()].emplace_back(cell);
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            uint_fast8_t numberIndex = index;
            numberIndex += doubleSize_;
            regions_[numberIndex] = Region(numberIndex, numberCells[index], size_);
        }
    }

    Cell& LatinSquare::minEntropyCell() noexcept {
        Cell* minCell = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (const auto& cell : entropyGrid_) {
            if (cell->filled()) {
                continue;
            }

            if (cell->entropy() == 0) {
                return *cell;
            }

            if (cell->entropy() < minEntropy) {
                minEntropy = cell->entropy();
                minCell = &(*cell);
            }
        }

        return *minCell;
    }

    Cell& LatinSquare::randomMinEntropyCell() noexcept {
        Cell* minCell = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (const auto& cell : entropyGrid_) {
            if (cell->filled()) {
                continue;
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

    const std::vector<uint_fast16_t> LatinSquare::update(Cell& cell, const uint_fast8_t number) noexcept {
        std::vector<uint_fast16_t> indexes;
        indexes.reserve((size_ - 1) << 1);
        uint_fast16_t rowIndex = cell.rawRow();
        rowIndex *= size_;
        uint_fast16_t columnIndex = cell.rawColumn();

        while (columnIndex < gridSize_) {
            if (grid_[rowIndex]->notFilled() && grid_[rowIndex]->remove(number)) {
                indexes.emplace_back(rowIndex);
            }

            if (grid_[columnIndex]->notFilled() && grid_[columnIndex]->remove(number)) {
                indexes.emplace_back(columnIndex);
            }

            ++rowIndex;
            columnIndex += size_;
        }

        return indexes;
    }

    Region& LatinSquare::minEntropyRegion() noexcept {
        Region* iterator = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (auto& region : regions_) {
            if (region.notEnabled()) {
                continue;
            }

            if (region.entropy() == 0) {
                return region;
            }

            if (region.entropy() < minEntropy) {
                minEntropy = region.entropy();
                iterator = &region;
            }
        }

        return *iterator;
    }

    Region& LatinSquare::randomMinEntropyRegion() noexcept {
        Region* iterator = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (auto& region : regions_) {
            if (region.notEnabled()) {
                continue;
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

    void LatinSquare::disable(const uint_fast16_t index) noexcept {
        grid_[index]->disable();
        regions_[grid_[index]->regionRow()].disableAndDecrease();
        regions_[grid_[index]->regionColumn()].disableAndDecrease();
        regions_[grid_[index]->regionNumber()].disableAndDecrease();
    }

    const std::vector<uint_fast16_t> LatinSquare::disableAndDecrease(const uint_fast16_t index) noexcept {
        std::vector<uint_fast16_t> indexes;
        indexes.reserve(regionsSize_ - 3);
        uint_fast16_t rowIndex = grid_[index]->rawRow();
        rowIndex *= size_;
        uint_fast16_t columnIndex = grid_[index]->rawColumn();

        while (columnIndex < gridSize_) {
            if (grid_[rowIndex]->enabled()) {
                grid_[rowIndex]->disable();
                regions_[grid_[rowIndex]->regionRow()].decrease();
                regions_[grid_[rowIndex]->regionColumn()].decrease();
                regions_[grid_[rowIndex]->regionNumber()].decrease();
                indexes.emplace_back(rowIndex);
            }

            if (grid_[columnIndex]->enabled()) {
                grid_[columnIndex]->disable();
                regions_[grid_[columnIndex]->regionRow()].decrease();
                regions_[grid_[columnIndex]->regionColumn()].decrease();
                regions_[grid_[columnIndex]->regionNumber()].decrease();
                indexes.emplace_back(columnIndex);
            }

            ++rowIndex;
            columnIndex += size_;
        }

        const auto numberIndexes = regions_[grid_[index]->regionNumber()].enabledCellIndexes();

        for (const auto numberIndex : numberIndexes) {
            grid_[numberIndex]->disable();
            regions_[grid_[numberIndex]->regionRow()].decrease();
            regions_[grid_[numberIndex]->regionColumn()].decrease();
            regions_[grid_[numberIndex]->regionNumber()].decrease();
            indexes.emplace_back(numberIndex);
        }

        return indexes;
    }

    void LatinSquare::enable(const uint_fast16_t index) noexcept {
        regions_[grid_[index]->regionRow()].enable();
        regions_[grid_[index]->regionColumn()].enable();
        regions_[grid_[index]->regionNumber()].enable();
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
