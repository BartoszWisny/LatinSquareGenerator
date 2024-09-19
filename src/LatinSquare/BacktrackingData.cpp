#include "BacktrackingData.hpp"

namespace LatinSquare {
    BacktrackingData::BacktrackingData(const Cell& cell, const EntropyData& entropyData)
        : filledCellId_(cell.getId()), previousEntropyData_(entropyData) {}

    const std::string& BacktrackingData::getFilledCellId() const {
        return filledCellId_;
    }

    const EntropyData& BacktrackingData::getPreviousEntropyData() const {
        return previousEntropyData_;
    }
}
