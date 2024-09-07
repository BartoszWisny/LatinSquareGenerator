#include "LatinSquareBacktrackingData.hpp"

namespace LatinSquareGenerator {
    BacktrackingData::BacktrackingData(const Cell& filledCell, const EntropyData& previousEntropyData) {
        setFilledCellId(filledCell.getId());
        setPreviousEntropyData(previousEntropyData);
    }

    const std::string& BacktrackingData::getFilledCellId() const {
        return filledCellId_;
    }

    void BacktrackingData::setFilledCellId(const std::string& filledCellId) {
        filledCellId_ = filledCellId;
    }

    const EntropyData& BacktrackingData::getPreviousEntropyData() const {
        return previousEntropyData_;
    }

    void BacktrackingData::setPreviousEntropyData(const EntropyData& previousEntropyData) {
        previousEntropyData_ = previousEntropyData;
    }
}
