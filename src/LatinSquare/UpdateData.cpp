#include "UpdateData.hpp"

namespace LatinSquare {
    UpdateData::UpdateData(const Cell& cell, const EntropyData& entropyData, const std::vector<std::string>& ids)
        : filledCellId_(cell.getId()), filledCellNumber_(cell.getNumber()), previousEntropyData_(entropyData),
          updatedCellsIds_(ids) {}

    const std::string& UpdateData::getFilledCellId() const {
        return filledCellId_;
    }

    int UpdateData::getFilledCellNumber() const {
        return filledCellNumber_;
    }

    const EntropyData& UpdateData::getPreviousEntropyData() const {
        return previousEntropyData_;
    }

    const std::vector<std::string>& UpdateData::getUpdatedCellsIds() const {
        return updatedCellsIds_;
    }
}
