#include "LatinSquare.hpp"

#include <memory>

namespace LatinSquare {
    LatinSquare::LatinSquare(const uint_fast8_t size, const Type type) noexcept
        : size_(size) {
        reset(type);
    }

    LatinSquare::LatinSquare(const uint_fast8_t size, const Type type, cpp::splitmix64& splitmix64) noexcept
        : size_(size), splitmix64_(splitmix64) {
        reset(type);
    }

    void LatinSquare::reset(const Type type) noexcept {
        gridSize_ = size_;
        gridSize_ *= size_;
        notFilled_ = gridSize_;
        grid_.resize(gridSize_);
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
    }

    void LatinSquare::setRegions() noexcept {
        regionsSize_ = 3 * size_;
        regions_.reserve(regionsSize_);
        std::vector<Region> columns, numbers;
        columns.reserve(size_);
        numbers.reserve(size_);
        std::vector<std::shared_ptr<Cell>> rowCells, columnCells, numberCells;
        rowCells.reserve(size_);
        columnCells.reserve(size_);
        numberCells.reserve(size_);

        for (uint_fast8_t index = 0; index < size_; ++index) {
            rowCells.clear();
            columnCells.clear();
            numberCells.clear();

            for (auto& cell : grid_) {
                if (cell->rawRow() == index) {
                    rowCells.emplace_back(cell);
                }

                if (cell->rawColumn() == index) {
                    columnCells.emplace_back(cell);
                }

                if (cell->number() == index) {
                    numberCells.emplace_back(cell);
                }
            }

            regions_.emplace_back(index, rowCells, size_);
            columns.emplace_back(index + size_, columnCells, size_);
            numbers.emplace_back(index + (size_ << 1), numberCells, size_);
        }

        regions_.insert(regions_.end(), columns.begin(), columns.end());
        regions_.insert(regions_.end(), numbers.begin(), numbers.end());
    }

    Cell& LatinSquare::minEntropyCell() noexcept {
        Cell* minCell = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (const auto& cell : grid_) {
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

        for (const auto& cell : grid_) {
            if (cell->filled()) {
                continue;
            }

            if (cell->entropy() < minEntropy) {
                minEntropy = cell->entropy();
                minCell = &(*cell);
            } else if (cell->entropy() == minEntropy && (splitmix64_.next() & 1)) {
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

        for (uint_fast8_t i = 0; i < size_; ++i) {
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
            } else if (region.entropy() == minEntropy && (splitmix64_.next() & 1)) {
                iterator = &region;
            }
        }

        return *iterator;
    }

    void LatinSquare::disable(const uint_fast16_t index) noexcept {
        grid_[index]->disable();
        regions_[grid_[index]->regionRow()].decrease();
        regions_[grid_[index]->regionColumn()].decrease();
        regions_[grid_[index]->regionNumber()].decrease();
        regions_[grid_[index]->regionRow()].disable();
        regions_[grid_[index]->regionColumn()].disable();
        regions_[grid_[index]->regionNumber()].disable();
    }

    const std::vector<uint_fast16_t> LatinSquare::disableAndDecrease(const uint_fast16_t index) noexcept {
        std::vector<uint_fast16_t> indexes;
        indexes.reserve(regionsSize_ - 3);
        uint_fast16_t rowIndex = grid_[index]->rawRow();
        rowIndex *= size_;
        uint_fast16_t columnIndex = grid_[index]->rawColumn();

        for (uint_fast8_t i = 0; i < size_; ++i) {
            if (grid_[rowIndex]->enabled() && rowIndex != index) {
                grid_[rowIndex]->disable();
                regions_[grid_[rowIndex]->regionRow()].decrease();
                regions_[grid_[rowIndex]->regionColumn()].decrease();
                regions_[grid_[rowIndex]->regionNumber()].decrease();
                indexes.emplace_back(rowIndex);
            }

            if (grid_[columnIndex]->enabled() && columnIndex != index) {
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
