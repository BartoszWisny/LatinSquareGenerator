#include "LatinSquareCell.hpp"

namespace LatinSquareGenerator {
    Cell::Cell(int row, int column, int maxEntropy) {
        setRow(row);
        setColumn(column);
        setId();
        setFullId();
        setMaxEntropy(maxEntropy);
        reset();
    }

    int Cell::getRow() const {
        return row_;
    }

    void Cell::setRow(int row) {
        row_ = row;
    }

    int Cell::getColumn() const {
        return column_;
    }

    void Cell::setColumn(int column) {
        column_ = column;
    }

    std::string Cell::getId() const {
        return id_;
    }

    void Cell::setId() {
        id_ = "R" + std::to_string(row_) +  "C" + std::to_string(column_);
    }

    int Cell::getNumber() const {
        return number_;
    }

    void Cell::setNumber(int number) {
        number_ = number;
    }

    void Cell::resetNumber() {
        setNumber(0);
    }

    std::string Cell::getFullId() const {
        return fullId_;
    }

    void Cell::setFullId() {
        fullId_ = id_ + "#" + std::to_string(number_);
    }

    void Cell::resetFullId() {
        fullId_ = id_ + "#0";
    }

    EntropyData Cell::getEntropyData() {
        return entropyData_;
    }

    void Cell::setEntropyData(EntropyData entropyData) {
        entropyData_ = entropyData;
    }

    void Cell::resetEntropyData() {
        entropyData_.resetEntropy();
        entropyData_.resetRemainingNumbers();
    }

    void Cell::clearEntropyData() {
        entropyData_.clearEntropy();
        entropyData_.clearRemainingNumbers();
    }

    int Cell::getEntropy() const {
        return entropyData_.getEntropy();
    }

    std::set<int> Cell::getRemainingNumbers() {
        return entropyData_.getRemainingNumbers();
    }

    void Cell::setMaxEntropy(int maxEntropy) {
        entropyData_.setMaxEntropy(maxEntropy);
    }

    void Cell::reset() {
        resetNumber();
        resetFullId();
        resetEntropyData();
    }

    void Cell::fill(int number) {
        setNumber(number);
        setFullId();
        clearEntropyData();
    }

    void Cell::clear(EntropyData previousEntropyData) {
        previousEntropyData.removeRemainingNumber(number_);
        resetNumber();
        resetFullId();
        setEntropyData(previousEntropyData);
    }

    bool Cell::removeRemainingNumber(int number) {
        return entropyData_.removeRemainingNumber(number);
    }

    void Cell::restoreRemainingNumber(int number) {
        entropyData_.restoreRemainingNumber(number);
    }
}
