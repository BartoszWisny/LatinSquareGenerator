#include "LatinSquareUpdateData.hpp"

namespace LatinSquareGenerator {
    UpdateData::UpdateData(const Cell& cell, const EntropyData& entropyData, const std::set<std::string>& ids) {
        setFilledCellId(cell.getId());
        setFilledCellNumber(cell.getNumber());
        setPreviousEntropyData(entropyData);
        setUpdatedCellsIds(ids);
    }

    const std::string& UpdateData::getFilledCellId() const {
        return filledCellId_;
    }

    void UpdateData::setFilledCellId(const std::string& id) {
        filledCellId_ = id;
    }

    int UpdateData::getFilledCellNumber() const {
        return filledCellNumber_;
    }

    void UpdateData::setFilledCellNumber(const int number) {
        filledCellNumber_ = number;
    }

    const EntropyData& UpdateData::getPreviousEntropyData() const {
        return previousEntropyData_;
    }

    void UpdateData::setPreviousEntropyData(const EntropyData& entropyData) {
        previousEntropyData_ = entropyData;
    }

    const std::set<std::string>& UpdateData::getUpdatedCellsIds() const {
        return updatedCellsIds_;
    }

    void UpdateData::setUpdatedCellsIds(const std::set<std::string>& ids) {
        updatedCellsIds_ = ids;
    }
}
