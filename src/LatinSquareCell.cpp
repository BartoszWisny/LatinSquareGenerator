#include "LatinSquareCell.hpp"

namespace LatinSquareGenerator {
    Cell::Cell(const int row, const int column, const int maxEntropy) {
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

    void Cell::setRow(const int row) {
        row_ = row;
    }

    int Cell::getColumn() const {
        return column_;
    }

    void Cell::setColumn(const int column) {
        column_ = column;
    }

    const std::string& Cell::getId() const {
        return id_;
    }

    void Cell::setId() {
        id_ = "R" + std::to_string(row_) +  "C" + std::to_string(column_);
    }

    int Cell::getNumber() const {
        return number_;
    }

    void Cell::setNumber(const int number) {
        number_ = number;
    }

    void Cell::resetNumber() {
        setNumber(0);
    }

    const std::string& Cell::getFullId() const {
        return fullId_;
    }

    void Cell::setFullId() {
        fullId_ = id_ + "#" + std::to_string(number_);
    }

    const EntropyData& Cell::getEntropyData() const {
        return entropyData_;
    }

    void Cell::setEntropyData(const EntropyData& entropyData) {
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

    const std::set<int>& Cell::getRemainingNumbers() const {
        return entropyData_.getRemainingNumbers();
    }

    void Cell::setMaxEntropy(const int maxEntropy) {
        entropyData_.setMaxEntropy(maxEntropy);
    }

    void Cell::reset() {
        resetNumber();
        setFullId();
        resetEntropyData();
    }

    void Cell::fill(const int number) {
        setNumber(number);
        setFullId();
        clearEntropyData();
    }

    void Cell::clear(EntropyData previousEntropyData) {
        previousEntropyData.removeRemainingNumber(number_);
        resetNumber();
        setFullId();
        setEntropyData(previousEntropyData);
    }

    bool Cell::removeRemainingNumber(const int number) {
        return entropyData_.removeRemainingNumber(number);
    }

    void Cell::restoreRemainingNumber(const int number) {
        entropyData_.restoreRemainingNumber(number);
    }
}
