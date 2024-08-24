#include "LatinSquareUpdateData.hpp"

namespace LatinSquareGenerator {
    UpdateData::UpdateData(
        const Cell& filledCell, const EntropyData& previousEntropyData, const std::set<std::string>& updatedCellsIds) {
        setFilledCellId(filledCell.getId());
        setFilledCellNumber(filledCell.getNumber());
        setPreviousEntropyData(previousEntropyData);
        setUpdatedCellsIds(updatedCellsIds);
    }

    const std::string& UpdateData::getFilledCellId() const {
        return filledCellId_;
    }

    void UpdateData::setFilledCellId(const std::string& filledCellId) {
        filledCellId_ = filledCellId;
    }

    int UpdateData::getFilledCellNumber() const {
        return filledCellNumber_;
    }

    void UpdateData::setFilledCellNumber(const int filledCellNumber) {
        filledCellNumber_ = filledCellNumber;
    }

    const EntropyData& UpdateData::getPreviousEntropyData() const {
        return previousEntropyData_;
    }

    void UpdateData::setPreviousEntropyData(const EntropyData& previousEntropyData) {
        previousEntropyData_ = previousEntropyData;
    }

    const std::set<std::string>& UpdateData::getUpdatedCellsIds() const {
        return updatedCellsIds_;
    }

    void UpdateData::setUpdatedCellsIds(const std::set<std::string>& updatedCellsIds) {
        updatedCellsIds_ = updatedCellsIds;
    }
}
