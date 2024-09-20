#include "Cell.hpp"

namespace LatinSquare {
    // consider using indexes and numbers starting from 0, not 1
    Cell::Cell(const int row, const int column, const bool reduced, const int latinSquareSize)
        : row_(row), column_(column), number_(0), latinSquareSize_(latinSquareSize), rowIdAsInt_(row_),
          columnIdAsInt_(latinSquareSize_ + column_), numberIdAsInt_(2 * latinSquareSize_ + number_),
          entropyData_(EntropyData(latinSquareSize_)), enabled_(true) {
        setIds();
        setFullId();
        reset(reduced);
    }

    int Cell::getRow() const {
        return row_;
    }

    int Cell::getColumn() const {
        return column_;
    }

    int Cell::getNumber() const {
        return number_;
    }

    void Cell::setNumber(const int number) {
        number_ = number;
        setFullId();
        setNumberIdAsInt();
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

    const std::string& Cell::getNumberId() const {
        return numberId_;
    }

    const std::string& Cell::getFullId() const {
        return fullId_;
    }

    int Cell::getRowIdAsInt() const {
        return rowIdAsInt_;
    }

    int Cell::getColumnIdAsInt() const {
        return columnIdAsInt_;
    }

    int Cell::getNumberIdAsInt() const {
        return numberIdAsInt_;
    }

    void Cell::setNumberIdAsInt() {
        numberIdAsInt_ = latinSquareSize_;
        numberIdAsInt_ <<= 1;
        numberIdAsInt_ += number_;
    }

    void Cell::setIds() {
        rowId_ = 'R';
        rowId_.append(std::to_string(row_));
        columnId_ = 'C';
        columnId_.append(std::to_string(column_));
        id_ = rowId_;
        id_.append(columnId_);
    }

    void Cell::setFullId() {
        numberId_ = '#';
        numberId_.append(std::to_string(number_));
        fullId_ = id_;
        fullId_.append(numberId_);
    }

    int Cell::getEntropy() const {
        return entropyData_.getEntropy();
    }

    const std::vector<int>& Cell::getRemainingNumbers() const {
        return entropyData_.getRemainingNumbers();
    }

    const EntropyData& Cell::getEntropyData() const {
        return entropyData_;
    }

    void Cell::setEntropyData(const EntropyData& entropyData) {
        entropyData_ = entropyData;
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
