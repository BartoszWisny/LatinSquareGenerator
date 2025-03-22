#include "Cell.hpp"

namespace LatinSquare {
    Cell::Cell(const uint_fast16_t index, const uint_fast8_t row, const uint_fast8_t column, const uint_fast8_t size,
        const Type type) noexcept
        : index_(index), rawRow_(row), rawColumn_(column), number_(EMPTY), size_(size), doubleSize_(size_ << 1),
          entropyData_(size_), maxNumber_(size_ - 1), regionRow_(rawRow_),
          regionColumn_(rawColumn_ + size_), regionNumber_(EMPTY), rowColumnSum_(row + column), enabled_(true),
          notOnDiagonal_(rawRow_ != rawColumn_) {
        reset(type);
    }

    constexpr void Cell::reset(const Type type) noexcept {
        if (type == Type::Reduced || type == Type::ReducedSymmetric) {
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
        }

        // TODO: check if these types are needed
        else if (type == Type::ReducedDiagonal) {
            // TODO: add reduced diagonal latin squares
        } else if (type == Type::ReducedSuperSymmetric) {
            // TODO: add super-symmetric latin squares
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
