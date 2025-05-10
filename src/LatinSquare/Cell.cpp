#include "Cell.hpp"

namespace LatinSquare {
    Cell::Cell(const uint_fast16_t index, const uint_fast8_t row, const uint_fast8_t column, const uint_fast8_t size,
        const Type type) noexcept
        : index_(index), rawRow_(row), rawColumn_(column), size_(size), doubleSize_(size_ << 1), type_(type),
          maxNumber_(size_ - 1), regionRow_(rawRow_), regionColumn_(rawColumn_ + size_), regionNumber_(EMPTY),
          rowColumnSum_(row + column), enabled_(true), notOnDiagonal_(rawRow_ != rawColumn_),
          notOnAntidiagonal_(rowColumnSum_ != maxNumber_), otherRegionIndexes_({rawRow_, rawColumn_}),
          triangularEnabled_({true, true}) {
        reset();
    }

    void Cell::reset() noexcept {
        number_ = EMPTY;
        entropyData_ = EntropyData(size_);

        if (type_ == Type::Reduced) {
            if (rawRow_ == 0) {
                fillAndClear(rawColumn_);
            } else if (rawColumn_ == 0) {
                fillAndClear(rawRow_);
            } else {
                entropyData_.remove(rawRow_);
                entropyData_.remove(rawColumn_);
            }
        } else if (type_ == Type::ReducedCyclic) {
            fillAndClear(rowColumnSum_ >= size_ ? rowColumnSum_ - size_ : rowColumnSum_);
        } else if (type_ == Type::ReducedDiagonal) {
            if (rawRow_ == 0) {
                fillAndClear(rawColumn_);
            } else if (rawColumn_ == 0) {
                fillAndClear(rawRow_);
            } else if ((size_ & 1) == 0 && !notOnDiagonal_) {
                fillAndClear(0);
            } /* else if ((size_ & 1) == 0 && !notOnAntidiagonal_) {
                fillAndClear(maxNumber_);
            } */ else {
                if ((size_ & 1) == 0) {
                    entropyData_.remove(0);
                    // entropyData_.remove(maxNumber_);
                }

                entropyData_.remove(rawRow_);
                entropyData_.remove(rawColumn_);
            }
        }
    }
}
