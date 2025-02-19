#include "Cell.hpp"

namespace LatinSquare {
    Cell::Cell(const uint_fast16_t index, const uint_fast8_t row, const uint_fast8_t column, const uint_fast8_t size,
        const Type type) noexcept
        : index_(index), rawRow_(row), rawColumn_(column), number_(EMPTY), size_(size),
          entropyData_(EntropyData(size_)), maxNumber_(size_ - 1), regionRow_(rawRow_),
          regionColumn_(rawColumn_ + size_), regionNumber_(EMPTY), rowColumnSum_(row + column), enabled_(true) {
        reset(type);
    }

    constexpr void Cell::reset(const Type type) noexcept {
        if (type == Type::Reduced) {
            if (rawRow_ == 0) {
                fill(rawColumn());
            } else if (rawColumn_ == 0) {
                fill(rawRow());
            } else {
                entropyData_.remove(rawRow());
                entropyData_.remove(rawColumn());
            }
        } else if (type == Type::ReducedCyclic) {
            fill(rowColumnSum_ % size_);
        } else if (type == Type::ReducedDiagonal) {
            if (rawRow_ == 0) {
                fill(rawColumn());
            } else if (rawColumn_ == 0) {
                fill(rawRow());
            } else if (rawRow() == rawColumn()) {
                fill(0);
            } else if (rowColumnSum_ == maxNumber_) {
                fill(maxNumber_);
            } else {
                entropyData_.remove(rawRow());
                entropyData_.remove(rawColumn());
                entropyData_.remove(0);
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
