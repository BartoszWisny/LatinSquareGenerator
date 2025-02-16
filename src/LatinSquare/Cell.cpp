#include "Cell.hpp"

namespace LatinSquare {
    Cell::Cell(const uint_fast16_t index, const uint_fast8_t row, const uint_fast8_t column, const uint_fast8_t size,
        const Type type) noexcept
        : index_(index), row_(row | ROW_FLAG), column_(column | COLUMN_FLAG), number_(EMPTY), rawRow_(row),
          rawColumn_(column), size_(size), entropyData_(EntropyData(size_)), maxNumber_(size_ - 1),
          rowColumnSum_(row + column), enabled_(true) {
        reset(type);
    }

    constexpr void Cell::reset(const Type type) noexcept {
        if (type == Type::Reduced) {
            if (row_ == ROW_FLAG) {
                fill(rawColumn());
            } else if (column_ == COLUMN_FLAG) {
                fill(rawRow());
            } else {
                entropyData_.remove(rawRow());
                entropyData_.remove(rawColumn());
            }
        } else if (type == Type::ReducedCyclic) {
            fill(rowColumnSum_ % size_);
        } else if (type == Type::ReducedDiagonal) {
            if (row_ == ROW_FLAG) {
                fill(rawColumn());
            } else if (column_ == COLUMN_FLAG) {
                fill(rawRow());
            } else if (rawRow() == rawColumn()) {
                fill(0x00);
            } else if (rowColumnSum_ == maxNumber_) {
                fill(maxNumber_);
            } else {
                entropyData_.remove(rawRow());
                entropyData_.remove(rawColumn());
                entropyData_.remove(0x00);
                entropyData_.remove(maxNumber_);
            }
        }
    }

    const std::string Cell::id() const noexcept {
        std::string id = "R";
        id += std::to_string(rawRow_);
        id += 'C';
        id += std::to_string(rawColumn_);
        id += '#';
        id += std::to_string(number_);
        return id;
    }
}
