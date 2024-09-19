#include "BacktrackingData.hpp"

namespace Transversal {
    BacktrackingData::BacktrackingData(const LatinSquare::Region& region, const LatinSquare::Cell& cell)
        : regionId_(region.getId()), chosenCellId_(cell.getId()) {}

    const std::string& BacktrackingData::getRegionId() const {
        return regionId_;
    }

    const std::string& BacktrackingData::getChosenCellId() const {
        return chosenCellId_;
    }
}
