#include "LatinSquareUtils.hpp"

#include <numeric>
#include <vector>

namespace LatinSquareGenerator {
    Position::Position() {}

    Position::Position(int row, int column) {
        setRow(row);
        setColumn(column);
    }

    int Position::getRow() {
        return row;
    }

    void Position::setRow(int row) {
        row = row;
    }

    int Position::getColumn() {
        return column;
    }

    void Position::setColumn(int column) {
        column = column;
    }

    EntropyData::EntropyData() {}

    EntropyData::EntropyData(int entropy, std::set<int> remainingNumbers) {
        setEntropy(entropy);
        setRemainingNumbers(remainingNumbers);
    }

    int EntropyData::getEntropy() {
        return entropy;
    }

    void EntropyData::setEntropy(int entropy) {
        entropy = entropy;
    }

    void EntropyData::resetEntropy() {
        setEntropy(maxEntropy);
    }

    void EntropyData::clearEntropy() {
        setEntropy(0);
    }

    std::set<int> EntropyData::getRemainingNumbers() {
        return remainingNumbers;
    }

    void EntropyData::setRemainingNumbers(std::set<int> remainingNumbers) {
        remainingNumbers = remainingNumbers;
    }

    void EntropyData::resetRemainingNumbers() {
        std::vector<int> remainingNumbersVector;
        std::iota(remainingNumbersVector.begin(), remainingNumbersVector.end(), 1);
        std::set<int> remainingNumbers(remainingNumbersVector.begin(), remainingNumbersVector.end());

        setRemainingNumbers(remainingNumbers);
    }

    void EntropyData::clearRemainingNumbers() {
        remainingNumbers.clear();
    }

    void EntropyData::setMaxEntropy(int maxEntropy) {
        maxEntropy = maxEntropy;
    }

    bool EntropyData::removeRemainingNumber(int number) {
        bool result = remainingNumbers.erase(number);

        if (result) {
            entropy--;
        }

        return result;
    }

    void EntropyData::restoreRemainingNumber(int number) {
        entropy++;
        remainingNumbers.insert(number);
    }

    FilledCellData::FilledCellData() {}

    FilledCellData::FilledCellData(Position position, int number, EntropyData previousEntropyData) {
        setPosition(position);
        setNumber(number);
        setPreviousEntropyData(previousEntropyData);
    }

    Position FilledCellData::getPosition() {
        return position;
    }

    void FilledCellData::setPosition(Position position) {
        position = position;
    }

    int FilledCellData::getNumber() {
        return number;
    }

    void FilledCellData::setNumber(int number) {
        number = number;
    }

    EntropyData FilledCellData::getPreviousEntropyData() {
        return previousEntropyData;
    }

    void FilledCellData::setPreviousEntropyData(EntropyData previousEntropyData) {
        previousEntropyData = previousEntropyData;
    }

    UpdateData::UpdateData() {}

    UpdateData::UpdateData(FilledCellData filledCellData, std::vector<Position> updatedCells) {
        setFilledCellData(filledCellData);
        setUpdatedCells(updatedCells);
    }

    FilledCellData UpdateData::getFilledCellData() {
        return filledCellData;
    }

    void UpdateData::setFilledCellData(FilledCellData filledCellData) {
        filledCellData = filledCellData;
    }

    std::vector<Position> UpdateData::getUpdatedCells() {
        return updatedCells;
    }

    void UpdateData::setUpdatedCells(std::vector<Position> updatedCells) {
        updatedCells = updatedCells;
    }
}
