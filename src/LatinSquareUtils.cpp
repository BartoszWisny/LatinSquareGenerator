#include "LatinSquareUtils.hpp"

#include <numeric>
#include <vector>

namespace LatinSquareGenerator {
    EntropyData::EntropyData() {}

    int EntropyData::getEntropy() const {
        return entropy_;
    }

    void EntropyData::setEntropy(int entropy) {
        entropy_ = entropy;
    }

    void EntropyData::resetEntropy() {
        setEntropy(maxEntropy_);
    }

    void EntropyData::clearEntropy() {
        setEntropy(0);
    }

    std::set<int> EntropyData::getRemainingNumbers() {
        return remainingNumbers_;
    }

    void EntropyData::setRemainingNumbers(std::set<int> remainingNumbers) {
        remainingNumbers_ = remainingNumbers;
    }

    void EntropyData::resetRemainingNumbers() {
        std::vector<int> remainingNumbersVector(maxEntropy_);
        std::iota(std::begin(remainingNumbersVector), std::end(remainingNumbersVector), 1);
        std::set<int> remainingNumbers(remainingNumbersVector.begin(), remainingNumbersVector.end());

        setRemainingNumbers(remainingNumbers);
    }

    void EntropyData::clearRemainingNumbers() {
        remainingNumbers_.clear();
    }

    void EntropyData::setMaxEntropy(int maxEntropy) {
        maxEntropy_ = maxEntropy;
    }

    bool EntropyData::removeRemainingNumber(int number) {
        auto result = remainingNumbers_.erase(number);

        if (result) {
            entropy_--;
        }

        return result;
    }

    void EntropyData::restoreRemainingNumber(int number) {
        entropy_++;
        remainingNumbers_.insert(number);
    }

    FilledCellData::FilledCellData() {}

    FilledCellData::FilledCellData(const std::string& id, const int number, const EntropyData& previousEntropyData) {
        setId(id);
        setNumber(number);
        setPreviousEntropyData(previousEntropyData);
    }

    std::string FilledCellData::getId() {
        return id_;
    }

    void FilledCellData::setId(std::string id) {
        id_ = id;
    }

    int FilledCellData::getNumber() {
        return number_;
    }

    void FilledCellData::setNumber(int number) {
        number_ = number;
    }

    EntropyData FilledCellData::getPreviousEntropyData() {
        return previousEntropyData_;
    }

    void FilledCellData::setPreviousEntropyData(EntropyData previousEntropyData) {
        previousEntropyData_ = previousEntropyData;
    }

    UpdateData::UpdateData() {}

    UpdateData::UpdateData(FilledCellData filledCellData, std::set<std::string> updatedCellsIds) {
        setFilledCellData(filledCellData);
        setUpdatedCellsIds(updatedCellsIds);
    }

    FilledCellData UpdateData::getFilledCellData() {
        return filledCellData_;
    }

    void UpdateData::setFilledCellData(FilledCellData filledCellData) {
        filledCellData_ = filledCellData;
    }

    std::set<std::string> UpdateData::getUpdatedCellsIds() {
        return updatedCellsIds_;
    }

    void UpdateData::setUpdatedCellsIds(std::set<std::string> updatedCellsIds) {
        updatedCellsIds_ = updatedCellsIds;
    }
}
