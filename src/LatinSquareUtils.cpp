#include "LatinSquareUtils.hpp"

#include <numeric>
#include <vector>

namespace LatinSquareGenerator {
    EntropyData::EntropyData() {}

    int EntropyData::getEntropy() const {
        return entropy_;
    }

    void EntropyData::setEntropy(const int entropy) {
        entropy_ = entropy;
    }

    void EntropyData::resetEntropy() {
        setEntropy(maxEntropy_);
    }

    void EntropyData::clearEntropy() {
        setEntropy(0);
    }

    const std::set<int>& EntropyData::getRemainingNumbers() const {
        return remainingNumbers_;
    }

    void EntropyData::setRemainingNumbers(const std::set<int>& remainingNumbers) {
        remainingNumbers_ = remainingNumbers;
    }

    void EntropyData::resetRemainingNumbers() {
        std::vector<int> remainingNumbersVector(maxEntropy_);
        std::iota(std::begin(remainingNumbersVector), std::end(remainingNumbersVector), 1);
        const std::set<int> remainingNumbers(remainingNumbersVector.begin(), remainingNumbersVector.end());

        setRemainingNumbers(remainingNumbers);
    }

    void EntropyData::clearRemainingNumbers() {
        remainingNumbers_.clear();
    }

    void EntropyData::setMaxEntropy(const int maxEntropy) {
        maxEntropy_ = maxEntropy;
    }

    bool EntropyData::removeRemainingNumber(const int number) {
        const auto result = remainingNumbers_.erase(number);

        if (result) {
            entropy_--;
        }

        return result;
    }

    void EntropyData::restoreRemainingNumber(const int number) {
        entropy_++;
        remainingNumbers_.insert(number);
    }

    FilledCellData::FilledCellData() {}

    FilledCellData::FilledCellData(const std::string& id, const int number, const EntropyData& previousEntropyData) {
        setId(id);
        setNumber(number);
        setPreviousEntropyData(previousEntropyData);
    }

    const std::string& FilledCellData::getId() const {
        return id_;
    }

    void FilledCellData::setId(const std::string& id) {
        id_ = id;
    }

    int FilledCellData::getNumber() const {
        return number_;
    }

    void FilledCellData::setNumber(const int number) {
        number_ = number;
    }

    const EntropyData& FilledCellData::getPreviousEntropyData() const {
        return previousEntropyData_;
    }

    void FilledCellData::setPreviousEntropyData(const EntropyData& previousEntropyData) {
        previousEntropyData_ = previousEntropyData;
    }

    UpdateData::UpdateData() {}

    UpdateData::UpdateData(const FilledCellData& filledCellData, const std::set<std::string>& updatedCellsIds) {
        setFilledCellData(filledCellData);
        setUpdatedCellsIds(updatedCellsIds);
    }

    const FilledCellData& UpdateData::getFilledCellData() const {
        return filledCellData_;
    }

    void UpdateData::setFilledCellData(const FilledCellData& filledCellData) {
        filledCellData_ = filledCellData;
    }

    const std::set<std::string>& UpdateData::getUpdatedCellsIds() const {
        return updatedCellsIds_;
    }

    void UpdateData::setUpdatedCellsIds(const std::set<std::string>& updatedCellsIds) {
        updatedCellsIds_ = updatedCellsIds;
    }
}
