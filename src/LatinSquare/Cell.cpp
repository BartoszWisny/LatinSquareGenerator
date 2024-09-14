#include "Cell.hpp"

#include <format>

namespace LatinSquare {
    Cell::Cell(const int row, const int column, const bool reduced, const int maxEntropy) {
        setRow(row);
        setColumn(column);
        setIds();
        setMaxEntropy(maxEntropy);
        reset(reduced);
        enable();
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

    int Cell::getNumber() const {
        return number_;
    }

    void Cell::setNumber(const int number) {
        number_ = number;
        setFullId();
    }

    const std::string& Cell::getRowId() const {
        return rowId_;
    }

    const std::string& Cell::getColumnId() const {
        return columnId_;
    }

    const std::string& Cell::getId() const {
        return id_;
    }

    void Cell::setIds() {
        rowId_ = std::format("R{}", row_);
        columnId_ = std::format("C{}", column_);
        id_ += rowId_;
        id_ += columnId_;
    }

    const std::string& Cell::getNumberId() const {
        return numberId_;
    }

    const std::string& Cell::getFullId() const {
        return fullId_;
    }

    void Cell::setFullId() {
        numberId_ = std::format("#{}", number_);
        fullId_ = id_;
        fullId_ += numberId_;
    }

    int Cell::getEntropy() const {
        return entropyData_.getEntropy();
    }

    const std::set<int>& Cell::getRemainingNumbers() const {
        return entropyData_.getRemainingNumbers();
    }

    const EntropyData& Cell::getEntropyData() const {
        return entropyData_;
    }

    void Cell::setEntropyData(const EntropyData& entropyData) {
        entropyData_ = entropyData;
    }

    void Cell::setMaxEntropy(const int maxEntropy) {
        entropyData_.setMaxEntropy(maxEntropy);
    }

    bool Cell::isEnabled() const {
        return enabled_;
    }

    void Cell::enable() {
        enabled_ = true;
    }

    void Cell::disable() {
        enabled_ = false;
    }

    void Cell::reset(const bool reduced) {
        if (reduced) {
            if (row_ == 1) {
                fill(column_);
            } else if (column_ == 1) {
                fill(row_);
            } else {
                setNumber(0);
                entropyData_.resetEntropyData();
                entropyData_.removeRemainingNumber(row_);
                entropyData_.removeRemainingNumber(column_);
            }
        } else {
            setNumber(0);
            entropyData_.resetEntropyData();
        }
    }

    void Cell::fill(const int number) {
        setNumber(number);
        entropyData_.clearEntropyData();
    }

    void Cell::clear(EntropyData entropyData) {
        entropyData.removeRemainingNumber(number_);

        setNumber(0);
        setEntropyData(entropyData);
    }

    bool Cell::removeRemainingNumber(const int number) {
        return entropyData_.removeRemainingNumber(number);
    }

    void Cell::restoreRemainingNumber(const int number) {
        entropyData_.restoreRemainingNumber(number);
    }
}
