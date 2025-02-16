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
        uint_fast16_t gridSize = size_;
        gridSize *= size_;
        grid_.resize(gridSize);
        notFilled_ = gridSize;
        std::shared_ptr<Cell> cell;
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;

        for (uint_fast16_t index = 0; index < gridSize; ++index) {
            cell = std::make_shared<Cell>(index, row, column, size_, type);
            grid_[index] = cell;

            if (cell->filled()) {
                --notFilled_;
            }

            if (++column == size_) {
                column = 0;
                ++row;
            }
        }
    }

    void LatinSquare::setRegions() noexcept {
        regions_.reserve(3 * size_);
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

            regions_.emplace_back(index, index | ROW_FLAG, rowCells, size_);
            columns.emplace_back(index + size_, index | COLUMN_FLAG, columnCells, size_);
            numbers.emplace_back(index + (size_ << 1), index, numberCells, size_);
        }

        regions_.insert(regions_.end(), columns.begin(), columns.end());
        regions_.insert(regions_.end(), numbers.begin(), numbers.end());
    }

    std::shared_ptr<Cell> LatinSquare::minEntropyCell() noexcept {
        std::shared_ptr<Cell> iterator = nullptr;
        uint_fast8_t minEntropy = 0xFF;
        uint_fast8_t entropy;

        for (auto& cell : grid_) {
            if (cell->filled()) {
                continue;
            }

            entropy = cell->entropy();

            if (entropy == 0) {
                return cell;
            }

            if (entropy < minEntropy) {
                minEntropy = entropy;
                iterator = cell;
            }
        }

        return iterator;
    }

    std::shared_ptr<Cell> LatinSquare::randomMinEntropyCell() noexcept {
        std::shared_ptr<Cell> iterator = nullptr;
        uint_fast8_t minEntropy = 0xFF;
        uint_fast8_t entropy;

        for (auto& cell : grid_) {
            if (cell->filled()) {
                continue;
            }

            entropy = cell->entropy();

            if (entropy < minEntropy) {
                minEntropy = entropy;
                iterator = cell;
            } else if (entropy == minEntropy) {
                if (splitmix64_.next() & 1) {
                    iterator = cell;
                }
            }
        }

        return iterator;
    }

    const std::vector<uint_fast16_t> LatinSquare::relatedToFilledCellIndexes(
        const std::shared_ptr<Cell> filledCell) const noexcept {
        std::vector<uint_fast16_t> indexes;
        indexes.reserve((size_ - 1) << 1);
        uint_fast16_t rowIndex = filledCell->rawRow();
        rowIndex *= size_;
        uint_fast16_t columnIndex = filledCell->rawColumn();

        for (uint_fast8_t i = 0; i < size_; ++i) {
            if (!grid_[rowIndex]->filled()) {
                indexes.emplace_back(rowIndex);
            }

            if (!grid_[columnIndex]->filled()) {
                indexes.emplace_back(columnIndex);
            }

            ++rowIndex;
            columnIndex += size_;
        }

        return indexes;
    }

    void LatinSquare::update(std::vector<uint_fast16_t>& indexes, const uint_fast8_t number) noexcept {
        auto partitionIndex = std::partition(indexes.begin(), indexes.end(),
                                             [this, number](const auto index) {
                                                 return grid_[index]->remove(number);
                                             });
        indexes.erase(partitionIndex, indexes.end());
    }

    Region& LatinSquare::minEntropyRegion() noexcept {
        Region* iterator = nullptr;
        uint_fast8_t minEntropy = 0xFF;
        uint_fast8_t entropy;

        for (auto& region : regions_) {
            if (region.notEnabled()) {
                continue;
            }

            entropy = region.entropy();

            if (entropy == 0) {
                return region;
            }

            if (entropy < minEntropy) {
                minEntropy = entropy;
                iterator = &region;
            }
        }

        return *iterator;
    }

    Region& LatinSquare::randomMinEntropyRegion() noexcept {
        Region* iterator = nullptr;
        uint_fast8_t minEntropy = 0xFF;
        uint_fast8_t entropy;

        for (auto& region : regions_) {
            if (region.notEnabled()) {
                continue;
            }

            entropy = region.entropy();

            if (entropy < minEntropy) {
                minEntropy = entropy;
                iterator = &region;
            } else if (entropy == minEntropy) {
                if (splitmix64_.next() & 1) {
                    iterator = &region;
                }
            }
        }

        return *iterator;
    }

    const std::vector<uint_fast16_t> LatinSquare::relatedToChosenCellIndexes(
        const std::shared_ptr<Cell> chosenCell) const noexcept {
        std::vector<uint_fast16_t> indexes;
        indexes.reserve(3 * size_ - 2);
        const auto row = chosenCell->row();
        const auto column = chosenCell->column();
        const auto number = chosenCell->number();

        for (auto& cell : grid_) {
            if (cell->enabled() && (cell->row() == row || cell->column() == column || cell->number() == number)) {
                indexes.emplace_back(cell->index());
            }
        }

        return indexes;
    }

    void LatinSquare::decrease(std::vector<uint_fast16_t>& indexes, const uint_fast16_t chosenIndex) noexcept {
        uint_fast64_t bits = 1ULL << size_;
        --bits;
        uint_fast64_t usedRows = bits, usedColumns = bits, usedNumbers = bits;
        const uint_fast8_t regionsSize = 3 * size_;
        std::vector<uint_fast8_t> regionIndexes;
        regionIndexes.reserve(regionsSize);
        std::vector<uint_fast8_t> regionUpdates(regionsSize, 0);
        auto cell = grid_[0];
        uint_fast8_t rowIndex, columnIndex, numberIndex;
        uint_fast64_t rowBit, columnBit, numberBit;

        for (const auto index : indexes) {
            cell = grid_[index];
            rowIndex = cell->rawRow();
            rowBit = 1ULL << rowIndex;
            columnIndex = cell->rawColumn();
            columnBit = 1ULL << columnIndex;
            columnIndex += size_;
            numberIndex = cell->number();
            numberBit = 1ULL << numberIndex;
            numberIndex += (size_ << 1);

            if (usedRows & rowBit) {
                regionIndexes.emplace_back(rowIndex);
                usedRows &= ~rowBit;
            }

            if (usedColumns & columnBit) {
                regionIndexes.emplace_back(columnIndex);
                usedColumns &= ~columnBit;
            }

            if (usedNumbers & numberBit) {
                regionIndexes.emplace_back(numberIndex);
                usedNumbers &= ~numberBit;
            }

            ++regionUpdates[rowIndex];
            ++regionUpdates[columnIndex];
            ++regionUpdates[numberIndex];
        }

        for (const auto index : regionIndexes) {
            regions_[index].decrease(regionUpdates[index]);
        }

        indexes.erase(std::remove(indexes.begin(), indexes.end(), chosenIndex), indexes.end());
    }

    void LatinSquare::disable(const uint_fast16_t index) noexcept {
        const auto cell = grid_[index];
        auto columnIndex = cell->rawColumn();
        columnIndex += size_;
        auto numberIndex = cell->number();
        numberIndex += (size_ << 1);
        regions_[cell->rawRow()].disable();
        regions_[columnIndex].disable();
        regions_[numberIndex].disable();
    }

    void LatinSquare::increase(const std::vector<uint_fast16_t>& indexes) noexcept {
        uint_fast64_t bits = 1ULL << size_;
        --bits;
        uint_fast64_t usedRows = bits, usedColumns = bits, usedNumbers = bits;
        const uint_fast8_t regionsSize = 3 * size_;
        std::vector<uint_fast8_t> regionIndexes;
        regionIndexes.reserve(regionsSize);
        std::vector<uint_fast8_t> regionUpdates(regionsSize, 0);
        auto cell = grid_[0];
        uint_fast8_t rowIndex, columnIndex, numberIndex;
        uint_fast64_t rowBit, columnBit, numberBit;

        for (const auto index : indexes) {
            cell = grid_[index];
            rowIndex = cell->rawRow();
            rowBit = 1ULL << rowIndex;
            columnIndex = cell->rawColumn();
            columnBit = 1ULL << columnIndex;
            columnIndex += size_;
            numberIndex = cell->number();
            numberBit = 1ULL << numberIndex;
            numberIndex += (size_ << 1);

            if (usedRows & rowBit) {
                regionIndexes.emplace_back(rowIndex);
                usedRows &= ~rowBit;
            }

            if (usedColumns & columnBit) {
                regionIndexes.emplace_back(columnIndex);
                usedColumns &= ~columnBit;
            }

            if (usedNumbers & numberBit) {
                regionIndexes.emplace_back(numberIndex);
                usedNumbers &= ~numberBit;
            }

            ++regionUpdates[rowIndex];
            ++regionUpdates[columnIndex];
            ++regionUpdates[numberIndex];
        }

        for (const auto index : regionIndexes) {
            regions_[index].increase(regionUpdates[index]);
        }
    }

    void LatinSquare::enable(const uint_fast16_t index) noexcept {
        const auto cell = grid_[index];
        auto columnIndex = cell->rawColumn();
        columnIndex += size_;
        auto numberIndex = cell->number();
        numberIndex += (size_ << 1);
        regions_[cell->rawRow()].enable();
        regions_[columnIndex].enable();
        regions_[numberIndex].enable();
    }
}
