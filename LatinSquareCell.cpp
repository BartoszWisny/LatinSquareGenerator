#include "LatinSquareCell.hpp"

#include <numeric>
#include <vector>

namespace LatinSquareGenerator {
    LatinSquareCell::LatinSquareCell(int latinSquareSize) {
        resetData();
        setMaxEntropy(latinSquareSize);
        resetPreviousEntropyData();
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

    int LatinSquareCell::getMaxEntropy() {
        return maxEntropy;
    }

    void LatinSquareCell::setMaxEntropy(int maxEntropy) {
        maxEntropy = maxEntropy;
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
        setEntropy(getMaxEntropy());
    }

    void LatinSquareCell::clearEntropy() {
        setEntropy(0);
    }

    std::set<int> LatinSquareCell::getRemainingNumbers() {
        return remainingNumbers;
    }

    void LatinSquareCell::setRemainingNumbers(std::set<int> remainingNumbers) {
        remainingNumbers = remainingNumbers;
    }

    void LatinSquareCell::resetRemainingNumbers() {
        std::vector<int> remainingNumbersVector;
        std::iota(remainingNumbersVector.begin(), remainingNumbersVector.end(), 1);
        std::set<int> remainingNumbers(remainingNumbersVector.begin(), remainingNumbersVector.end());

        setRemainingNumbers(remainingNumbers);
    }

    void LatinSquareCell::clearRemainingNumbers() {
        setRemainingNumbers(std::set<int>());
    }

    void LatinSquareCell::setData(int number, int entropy, std::set<int> remainingNumbers) {
        setNumber(number);
        setEntropy(entropy);
        setRemainingNumbers(remainingNumbers);
    }

    void LatinSquareCell::resetData() {
        resetNumber();
        resetEntropy();
        resetRemainingNumbers();
    }

    void LatinSquareCell::clearEntropyData() {
        clearEntropy();
        clearRemainingNumbers();
    }

    int LatinSquareCell::getPreviousEntropy() {
        return previousEntropy;
    }

    void LatinSquareCell::setPreviousEntropy(int previousEntropy) {
        previousEntropy = previousEntropy;
    }

    void LatinSquareCell::resetPreviousEntropy() {
        setPreviousEntropy(getMaxEntropy());
    }

    std::set<int> LatinSquareCell::getPreviousRemainingNumbers() { // maybe won't be enough
        return previousRemainingNumbers;
    }

    void LatinSquareCell::setPreviousRemainingNumbers(std::set<int> previousRemainingNumbers) {
        previousRemainingNumbers = previousRemainingNumbers;
    }

    void LatinSquareCell::resetPreviousRemainingNumbers() {
        std::vector<int> previousRemainingNumbersVector;
        std::iota(previousRemainingNumbersVector.begin(), previousRemainingNumbersVector.end(), 1);
        std::set<int> previousRemainingNumbers(previousRemainingNumbersVector.begin(),
                                               previousRemainingNumbersVector.end());

        setPreviousRemainingNumbers(previousRemainingNumbers);
    }

    void LatinSquareCell::setPreviousEntropyData(int previousEntropy, std::set<int> previousRemainingNumbers) {
        setPreviousEntropy(previousEntropy);
        setPreviousRemainingNumbers(previousRemainingNumbers);
    }

    void LatinSquareCell::resetPreviousEntropyData() {
        resetPreviousEntropy();
        resetPreviousRemainingNumbers();
    }

    void LatinSquareCell::rememberPreviousEntropyData() {
        setPreviousEntropy(getEntropy());
        setPreviousRemainingNumbers(getRemainingNumbers());
    }

    void LatinSquareCell::restorePreviousEntropyData() {
        setEntropy(getPreviousEntropy());
        setRemainingNumbers(getPreviousRemainingNumbers());
    }

    void LatinSquareCell::fill(int number) {
        setNumber(number);
        rememberPreviousEntropyData();
        clearEntropyData();
    }

    void LatinSquareCell::clear() {
        resetNumber();
        restorePreviousEntropyData();
    }

    // void LatinSquareCell::removeRemainingNumber(int number) {
    //     std::set<int> remainingNumbers = getRemainingNumbers();
    //     remainingNumbers.erase(number);

    //     setRemainingNumbers(remainingNumbers);
    // }

    // void LatinSquareCell::restoreRemainingNumber(int number) {
    //     std::set<int> remainingNumbers = getRemainingNumbers();
    //     remainingNumbers.insert(number);

    //     setRemainingNumbers(remainingNumbers);
    // }
}
