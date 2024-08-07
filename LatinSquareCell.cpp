#include "LatinSquareCell.hpp"

#include <numeric>

namespace LatinSquareGenerator {
    LatinSquareCell::LatinSquareCell(int latinSquareSize) {
        resetNumber();
        resetEntropy();
        resetRemainingNumbers();
        setLatinSquareSize(latinSquareSize);
    }

    int LatinSquareCell::getRow() {
        return row;
    }

    void LatinSquareCell::setRow(int row) {
        row = row;
    }

    int LatinSquareCell::getColumn() {
        return column;
    }

    void LatinSquareCell::setColumn(int column) {
        column = column;
    }

    void LatinSquareCell::setPosition(int row, int column) {
        setRow(row);
        setColumn(column);
    }

    int LatinSquareCell::getNumber() {
        return number;
    }

    void LatinSquareCell::setNumber(int number) {
        number = number;
    }

    void LatinSquareCell::resetNumber() {
        setNumber(0);
    }

    int LatinSquareCell::getEntropy() {
        return entropy;
    }

    void LatinSquareCell::setEntropy(int entropy) {
        entropy = entropy;
    }

    void LatinSquareCell::resetEntropy() {
        setEntropy(getLatinSquareSize());
    }

    std::vector<int> LatinSquareCell::getRemainingNumbers() {
        return remainingNumbers;
    }

    void LatinSquareCell::setRemainingNumbers(std::vector<int> remainingNumbers) {
        remainingNumbers = remainingNumbers;
    }

    void LatinSquareCell::resetRemainingNumbers() {
        std::vector<int> remainingNumbers(getLatinSquareSize());
        std::iota(remainingNumbers.begin(), remainingNumbers.end(), 1);

        setRemainingNumbers(remainingNumbers);
    }

    void LatinSquareCell::setData(int number, int entropy, std::vector<int> remainingNumbers) {
        setNumber(number);
        setEntropy(entropy);
        setRemainingNumbers(remainingNumbers);
    }

    void LatinSquareCell::resetData() {
        resetNumber();
        resetEntropy();
    }

    int LatinSquareCell::getLatinSquareSize() {
        return latinSquareSize;
    }

    void LatinSquareCell::setLatinSquareSize(int latinSquareSize) {
        latinSquareSize = latinSquareSize;
    }

    // void LatinSquareCell::fill(int number) {
    //     // TODO: fill cell with number, reset entropy and remainingNumbers
    // }

    // void LatinSquareCell::removeRemainingNumber(int number) {
    //     // TODO: remove number from remainingNumbers
    // }
}
