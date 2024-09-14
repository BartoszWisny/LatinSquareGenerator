#include "LatinSquareBacktrackingData.hpp"

namespace LatinSquareGenerator {
    BacktrackingData::BacktrackingData(const Cell& cell, const EntropyData& entropyData) {
        setFilledCellId(cell.getId());
        setPreviousEntropyData(entropyData);
    }

    const std::string& BacktrackingData::getFilledCellId() const {
        return filledCellId_;
    }

    void BacktrackingData::setFilledCellId(const std::string& id) {
        filledCellId_ = id;
    }

    const EntropyData& BacktrackingData::getPreviousEntropyData() const {
        return previousEntropyData_;
    }

    void BacktrackingData::setPreviousEntropyData(const EntropyData& entropyData) {
        previousEntropyData_ = entropyData;
    }
}
