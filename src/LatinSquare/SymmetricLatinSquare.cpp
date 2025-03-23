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

        grid_.clear();
        triangularGrid_.clear();
        entropyTriangularGrid_.clear();
        triangularRegions_.clear();
        gridSize_ = size_;
        gridSize_ *= size_;
        triangularGridSize_ = gridSize_;
        triangularGridSize_ += size_;
        triangularGridSize_ >>= 1;
        entropyTriangularGridSize_ = triangularGridSize_;
        entropyTriangularGridSize_ -= size_;
        notFilled_ = entropyTriangularGridSize_;
        grid_.resize(gridSize_);
        triangularGrid_.resize(triangularGridSize_);
        entropyTriangularGrid_.reserve(entropyTriangularGridSize_);
        triangularRegions_.reserve(size_);
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
            }

            if (row > column) {
                if (grid_[index]->filled()) {
                    --notFilled_;
                } else {
                    entropyTriangularGrid_.emplace_back(grid_[index]);
                }
            }

            if (++column == size_) {
                column = 0;
                ++row;
            }
        }

        for (uint_fast8_t index = 0; index < size_; ++index) {
            triangularRegions_.emplace_back(index, cells[index], size_);
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

        grid_.clear();
        triangularGrid_.clear();
        entropyTriangularGrid_.clear();
        triangularRegions_.clear();
        gridSize_ = size_;
        gridSize_ *= size_;
        triangularGridSize_ = gridSize_;
        triangularGridSize_ += size_;
        triangularGridSize_ >>= 1;
        grid_.resize(gridSize_);
        triangularGrid_.resize(triangularGridSize_);
        triangularRegions_.reserve(size_);
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

    // void LatinSquare::setRegions() noexcept {
    //     for (auto& cell : grid_) {
    //         cell->enable();
    //     }

    //     regions_.clear();
    //     regionsSize_ = 3 * size_;
    //     regions_.reserve(regionsSize_);
    //     std::vector<Region> columns, numbers;
    //     columns.reserve(size_);
    //     numbers.reserve(size_);
    //     std::vector<std::shared_ptr<Cell>> rowCells, columnCells, numberCells;
    //     rowCells.reserve(size_);
    //     columnCells.reserve(size_);
    //     numberCells.reserve(size_);

    //     for (uint_fast8_t index = 0; index < size_; ++index) {
    //         rowCells.clear();
    //         columnCells.clear();
    //         numberCells.clear();

    //         for (auto& cell : grid_) {
    //             if (cell->rawRow() == index) {
    //                 rowCells.emplace_back(cell);
    //             }

    //             if (cell->rawColumn() == index) {
    //                 columnCells.emplace_back(cell);
    //             }

    //             if (cell->number() == index) {
    //                 numberCells.emplace_back(cell);
    //             }
    //         }

    //         regions_.emplace_back(index, rowCells, size_);
    //         columns.emplace_back(index + size_, columnCells, size_);
    //         numbers.emplace_back(index + (size_ << 1), numberCells, size_);
    //     }

    //     regions_.insert(
    //         regions_.end(), std::make_move_iterator(columns.begin()), std::make_move_iterator(columns.end()));
    //     regions_.insert(
    //         regions_.end(), std::make_move_iterator(numbers.begin()), std::make_move_iterator(numbers.end()));
    // }

    Cell& SymmetricLatinSquare::minEntropyCell() noexcept {
        Cell* minCell = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (const auto& cell : entropyTriangularGrid_) {
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

    Cell& SymmetricLatinSquare::randomMinEntropyCell() noexcept {
        Cell* minCell = nullptr;
        uint_fast8_t minEntropy = 0xFF;

        for (const auto& cell : entropyTriangularGrid_) {
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

    const std::vector<uint_fast16_t> SymmetricLatinSquare::update(Cell& cell, const uint_fast8_t number) noexcept {
        std::vector<uint_fast16_t> indexes;
        indexes.reserve((size_ - 1) << 1);
        const auto rowCellIndexes = triangularRegions_[cell.rawRow()].notFilledCellIndexes();
        const auto columnCellIndexes = triangularRegions_[cell.rawColumn()].notFilledCellIndexes();

        for (const auto index : rowCellIndexes) {
            if (grid_[index]->remove(number)) {
                indexes.emplace_back(index);
            }
        }

        for (const auto index : columnCellIndexes) {
            if (grid_[index]->remove(number)) {
                indexes.emplace_back(index);
            }
        }

        return indexes;
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
        uint_fast16_t index = 0;
        uint_fast16_t triangularIndex = 1;

        if (grid_[0]->notFilled()) {
            grid_[0]->fill(triangularGrid_[0]->numbers()[0]);
        }

        for (uint_fast8_t columnSize = 1, columnIndex = 1; columnSize < size_; ++columnSize, ++columnIndex) {
            index = columnIndex;

            for (uint_fast8_t counter = 0; counter < columnSize; ++counter, index += size_, ++triangularIndex) {
                if (grid_[index]->notFilled()) {
                    grid_[index]->fill(triangularGrid_[triangularIndex]->number());
                }
            }

            if (grid_[index]->notFilled()) {
                grid_[index]->fill(triangularGrid_[triangularIndex]->numbers()[0]);
            }

            ++triangularIndex;
        }
    }

    // Region& LatinSquare::minEntropyRegion() noexcept {
    //     Region* iterator = nullptr;
    //     uint_fast8_t minEntropy = 0xFF;

    //     for (auto& region : regions_) {
    //         if (region.notEnabled()) {
    //             continue;
    //         }

    //         if (region.entropy() == 0) {
    //             return region;
    //         }

    //         if (region.entropy() < minEntropy) {
    //             minEntropy = region.entropy();
    //             iterator = &region;
    //         }
    //     }

    //     return *iterator;
    // }

    // Region& LatinSquare::randomMinEntropyRegion() noexcept {
    //     Region* iterator = nullptr;
    //     uint_fast8_t minEntropy = 0xFF;

    //     for (auto& region : regions_) {
    //         if (region.notEnabled()) {
    //             continue;
    //         }

    //         if (region.entropy() < minEntropy) {
    //             minEntropy = region.entropy();
    //             iterator = &region;
    //         } else if (region.entropy() == minEntropy && (splitmix64_.next() & 1)) { // TODO: maybe use better randomness
    //             iterator = &region;
    //         }
    //     }

    //     return *iterator;
    // }

    // void LatinSquare::disable(const uint_fast16_t index) noexcept {
    //     grid_[index]->disable();
    //     regions_[grid_[index]->regionRow()].decrease();
    //     regions_[grid_[index]->regionColumn()].decrease();
    //     regions_[grid_[index]->regionNumber()].decrease();
    //     regions_[grid_[index]->regionRow()].disable();
    //     regions_[grid_[index]->regionColumn()].disable();
    //     regions_[grid_[index]->regionNumber()].disable();
    // }

    // const std::vector<uint_fast16_t> LatinSquare::disableAndDecrease(const uint_fast16_t index) noexcept {
    //     std::vector<uint_fast16_t> indexes;
    //     indexes.reserve(regionsSize_ - 3);
    //     uint_fast16_t rowIndex = grid_[index]->rawRow();
    //     rowIndex *= size_;
    //     uint_fast16_t columnIndex = grid_[index]->rawColumn();

    //     for (uint_fast8_t i = 0; i < size_; ++i) {
    //         if (grid_[rowIndex]->enabled() && rowIndex != index) {
    //             grid_[rowIndex]->disable();
    //             regions_[grid_[rowIndex]->regionRow()].decrease();
    //             regions_[grid_[rowIndex]->regionColumn()].decrease();
    //             regions_[grid_[rowIndex]->regionNumber()].decrease();
    //             indexes.emplace_back(rowIndex);
    //         }

    //         if (grid_[columnIndex]->enabled() && columnIndex != index) {
    //             grid_[columnIndex]->disable();
    //             regions_[grid_[columnIndex]->regionRow()].decrease();
    //             regions_[grid_[columnIndex]->regionColumn()].decrease();
    //             regions_[grid_[columnIndex]->regionNumber()].decrease();
    //             indexes.emplace_back(columnIndex);
    //         }

    //         ++rowIndex;
    //         columnIndex += size_;
    //     }

    //     const auto numberIndexes = regions_[grid_[index]->regionNumber()].enabledCellIndexes();

    //     for (const auto numberIndex : numberIndexes) {
    //         grid_[numberIndex]->disable();
    //         regions_[grid_[numberIndex]->regionRow()].decrease();
    //         regions_[grid_[numberIndex]->regionColumn()].decrease();
    //         regions_[grid_[numberIndex]->regionNumber()].decrease();
    //         indexes.emplace_back(numberIndex);
    //     }

    //     return indexes;
    // }

    // void LatinSquare::enable(const uint_fast16_t index) noexcept {
    //     regions_[grid_[index]->regionRow()].enable();
    //     regions_[grid_[index]->regionColumn()].enable();
    //     regions_[grid_[index]->regionNumber()].enable();
    // }

    // void LatinSquare::enableAndIncrease(const std::vector<uint_fast16_t>& indexes) noexcept {
    //     for (const auto index : indexes) {
    //         grid_[index]->enable();
    //         regions_[grid_[index]->regionRow()].increase();
    //         regions_[grid_[index]->regionColumn()].increase();
    //         regions_[grid_[index]->regionNumber()].increase();
    //     }
    // }
}
