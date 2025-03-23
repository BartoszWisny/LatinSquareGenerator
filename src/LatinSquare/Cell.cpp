#include "Cell.hpp"

namespace LatinSquare {
    Cell::Cell(const uint_fast16_t index, const uint_fast8_t row, const uint_fast8_t column, const uint_fast8_t size,
        const Type type) noexcept
        : index_(index), rawRow_(row), rawColumn_(column), size_(size), doubleSize_(size_ << 1), type_(type),
          maxNumber_(size_ - 1), regionRow_(rawRow_), regionColumn_(rawColumn_ + size_), regionNumber_(EMPTY),
          rowColumnSum_(row + column), enabled_(true), notOnDiagonal_(rawRow_ != rawColumn_) {
        reset();
    }

    void Cell::reset() noexcept {
        number_ = EMPTY;
        entropyData_ = EntropyData(size_);

        if (type_ == Type::Reduced || type_ == Type::ReducedSymmetric) {
            if (rawRow_ == 0) {
                fillAndClear(rawColumn());
            } else if (rawColumn_ == 0) {
                fillAndClear(rawRow());
            } else {
                entropyData_.remove(rawRow());
                entropyData_.remove(rawColumn());
            }
        } else if (type_ == Type::ReducedCyclic) {
            fillAndClear(rowColumnSum_ % size_);
        }

        // TODO: check if these types are needed
        else if (type_ == Type::ReducedDiagonal) {
            // TODO: add reduced diagonal latin squares
        } else if (type_ == Type::ReducedSuperSymmetric) {
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
