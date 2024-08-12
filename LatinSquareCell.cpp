#include "LatinSquareCell.hpp"

namespace LatinSquareGenerator {
    Cell::Cell(int row, int column, int maxEntropy) {
        setPosition(row, column);
        reset();
        setMaxEntropy(maxEntropy);
    }

    Position Cell::getPosition() {
        return position;
    }

    void Cell::setPosition(int row, int column) {
        position = Position(row, column);
    }

    int Cell::getRow() {
        return position.getRow();
    }

    int Cell::getColumn() {
        return position.getColumn();
    }

    int Cell::getNumber() {
        return number;
    }

    void Cell::setNumber(int number) {
        number = number;
    }

    void Cell::resetNumber() {
        setNumber(0);
    }

    EntropyData Cell::getEntropyData() {
        return entropyData;
    }

    void Cell::setEntropyData(EntropyData entropyData) {
        entropyData = entropyData;
    }

    void Cell::resetEntropyData() {
        entropyData.resetEntropy();
        entropyData.resetRemainingNumbers();
    }

    void Cell::clearEntropyData() {
        entropyData.clearEntropy();
        entropyData.clearRemainingNumbers();
    }

    int Cell::getEntropy() {
        return entropyData.getEntropy();
    }

    std::set<int> Cell::getRemainingNumbers() {
        return entropyData.getRemainingNumbers();
    }

    void Cell::setMaxEntropy(int maxEntropy) {
        entropyData.setMaxEntropy(maxEntropy);
    }

    void Cell::reset() {
        resetNumber();
        resetEntropyData();
    }

    void Cell::fill(int number) {
        setNumber(number);
        clearEntropyData();
    }

    void Cell::clear(EntropyData previousEntropyData) {
        previousEntropyData.removeRemainingNumber(number);
        resetNumber();
        setEntropyData(previousEntropyData);
    }

    bool Cell::removeRemainingNumber(int number) {
        return entropyData.removeRemainingNumber(number);
    }

    void Cell::restoreRemainingNumber(int number) {
        entropyData.restoreRemainingNumber(number);
    }
}
